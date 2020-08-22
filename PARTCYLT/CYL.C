

//#include "\engine\engine.h"
//#include "\engine\keyframe.h"
//#include "\engine\dloader.h"


#define textile 1.5



#define scr ((char*)0xa0000)

#include        <assert.h>
#include        <math.h>
#include        <stdlib.h>

#include        "..\main.h"
#include        "..\scriptde.h"
#include        "cylasm.h"
#include        "mtrace.h"
#include        "..\demolib\layer.h"
#include        "..\demolib\komms.h"
#include        "..\demolib\vesavbe.h"
#include        "..\demolib\col256.h"
#include        "..\demolib\convert.h"
#include        "..\demolib\matrix.h"
#include        "..\demolib\clockcyc.h"
#include        "..\demolib\binseq.h"
#include        "..\demolib\spline.h"
#include        "..\demolib\pcx.h"

#include        "cylcalc.h"

#define trace_dimx 81
#define trace_dimy 51


static rgb1 fake[64000];

cc_timer        trace_tim1,trace_tim2,trace_tim3,trace_tim4;

char    *trace_texture;

float lysx,lysy,lysz;

trace_p trace_screen[trace_dimy][trace_dimx];

float trace_posx,trace_posy,trace_posz,trace_rotx,trace_roty,trace_rotz;

spline_key *trace_splinex,*trace_spliney,*trace_splinez;

int     trace_lystabel[1025];

#define cyldim 20

typedef struct cylinder{
        int     o;
        float   x,y,r,r2;
        float   gamma;
        struct cylinder *nabox[cyldim],*naboy[cyldim];
        int     texnum;
} cylinder;


typedef struct sphere{
        float x,y,z,r;

        int     ncyl;
        struct cyl2 *cyl[10];
        int     num;

} sphere;

typedef struct cyl2{
        float x,y,z,u,v,w,r;
        sphere *s[cyldim];
        int len;
        int     num;
}cyl2;


        cyl2    cyl[25];
        sphere  s[23];


#define         ntunnels 7

cylinder        tunnels[ntunnels];

#define sqrt_scale 0.10
#define sqrt_size 1024

float   sqrt_tabel[sqrt_size+1];

#define atan_scale 64.0
#define atan_size 128*2

float   atan_tabel[atan_size+1];

static int num;

static void setcyl(cyl2 *c, float x, float y, float z, float u, float v, float w, float r, int len)
{
        float l=sqrt(u*u+v*v+w*w);
        int t;

        c->x=x;
        c->y=y;
        c->z=z;
        c->u=u/l;
        c->v=v/l;
        c->w=w/l;
        c->r=r;
        c->len=len;

        c->num=num+=16;

        for(t=0;t<cyldim;t++)c->s[t]=NULL;
}

static void setcylrel(cyl2 *c, sphere *s, float u, float v, float w, float r, int len)
{
        float l=sqrt(u*u+v*v+w*w);
        int t;

        c->x=s->x;
        c->y=s->y;
        c->z=s->z;
        c->u=u/l;
        c->v=v/l;
        c->w=w/l;
        c->r=r;
        c->len=len;

        c->num=num+=16;

        for(t=0;t<cyldim;t++)c->s[t]=NULL;
}

static void setsphere(sphere *c, float x, float y, float z, float r)
{
        c->x=x;
        c->y=y;
        c->z=z;
        c->r=r;
        c->ncyl=0;

        c->num=num+=16;
}

static void setsphererel(sphere *s, cyl2 *c, float dist, float r)
{
        s->x=c->x+c->u*dist;
        s->y=c->y+c->v*dist;
        s->z=c->z+c->w*dist;
        s->r=r;
        s->ncyl=0;

        s->num=num+=16;
}

static float intersectcyl(cyl2 *cyl, float a, float b, float c)
{
        float s=a*cyl->u+b*cyl->v+c*cyl->w;
        float t=cyl->x*cyl->u+cyl->y*cyl->v+cyl->z*cyl->w;

        float A=a*a+b*b+c*c-s*s;
        float B=-2*a*cyl->x-2*b*cyl->y-2*c*cyl->z+2*s*t;
        float C=cyl->x*cyl->x+cyl->y*cyl->y+cyl->z*cyl->z-cyl->r*cyl->r-t*t;

        float D=B*B-4*A*C;

        return (-B+sqrt(D))/(2*A);
}

static float intersectsphere(sphere *s, float a, float b, float c)
{
        float A=a*a+b*b+c*c;
        float B=-2*a*s->x-2*b*s->y-2*c*s->z;
        float C=s->x*s->x+s->y*s->y+s->z*s->z-s->r*s->r;

        float D=B*B-4*A*C;

        return (-B+sqrt(D))/(2*A);
}

static int insidesphere(sphere *s, float a, float b, float c)
{
        return ((s->x-a)*(s->x-a)+(s->y-b)*(s->y-b)+(s->z-c)*(s->z-c)<s->r*s->r);
}

static void addsphere(sphere *s, cyl2 *c)
{
        int t;
        for(t=0;t<cyldim;t++)
        {
                if(insidesphere(s,c->x+t*c->u,c->y+t*c->v,c->z+t*c->w)||insidesphere(s,c->x+(t+1)*c->u,c->y+(t+1)*c->v,c->z+(t+1)*c->w))
                {
                        c->s[t]=s;
                }
        }

        if(s->ncyl>9)
        {
                printf("for mange cyl.\n");
                exit(1);
        }
        s->cyl[s->ncyl]=c;
        s->ncyl++;
}


__inline int     trace_findbesttunnel(float x, float y, float z, float dx, float dy, float dz)
{
        int     best,a;
        float   bestv;

        best=0;
        bestv=-1;

        for(a=1;a<ntunnels;a++)
        {
                switch(tunnels[a].o)
                {
                        case 0:
                                if(((x-tunnels[a].x)*(x-tunnels[a].x)+(y-tunnels[a].y)*(y-tunnels[a].y)<tunnels[a].r2)&&(bestv<dz*dz))
                                {
                                        bestv=dz*dz;
                                        best=a;
                                }
                                break;
                        case 1:
                                if(((y-tunnels[a].x)*(y-tunnels[a].x)+(z-tunnels[a].y)*(z-tunnels[a].y)<tunnels[a].r2)&&(bestv<dx*dx))
                                {
                                        bestv=dx*dx;
                                        best=a;
                                }
                                break;
                        case 2:
                                if(((z-tunnels[a].x)*(z-tunnels[a].x)+(x-tunnels[a].y)*(x-tunnels[a].y)<tunnels[a].r2)&&(bestv<dy*dy))
                                {
                                        bestv=dy*dy;
                                        best=a;
                                }
                                break;
                }
        }
        return best;
}


void    trace_initlystabel()
{
        int     t;

        for(t=0;t<1025;t++)
        {
                float   f;

                f=t/1024.0;
               // trace_lystabel[t]=(0.75+sqrt(sqrt(f)))*64*128*2*1.4;
                trace_lystabel[t]=(0.75+((f*f*f)))*64*128*2*1.4;
        }
}

void    trace_calculateneighbourinfo()
{
        int     a,b,z;
        int     o;

        for(z=0;z<cyldim;z++)tunnels[0].nabox[z]=tunnels[0].naboy[z]=0;

        for(a=1;a<ntunnels;a++)tunnels[a].r2=tunnels[a].r*tunnels[a].r;

        for(a=1;a<ntunnels;a++)
        {

                for(z=0;z<cyldim;z++)
                {
                        tunnels[a].nabox[z]=0;
                        tunnels[a].naboy[z]=0;

                        for(b=1;b<ntunnels;b++)
                        {
                                o=(3+(tunnels[b].o-tunnels[a].o))%3;

                                if(o==1)
                                {
                                        if((tunnels[b].y+tunnels[b].r>z)&&(tunnels[b].y-tunnels[b].r<z+1)&&((tunnels[b].x-tunnels[a].y)*(tunnels[b].x-tunnels[a].y)<(tunnels[b].r+tunnels[a].r)*(tunnels[b].r+tunnels[a].r)))
                                        tunnels[a].nabox[z]=&tunnels[b];
                                }
                                if(o==2)
                                {
                                        if((tunnels[b].x+tunnels[b].r>z)&&(tunnels[b].x-tunnels[b].r<z+1)&&((tunnels[b].y-tunnels[a].x)*(tunnels[b].y-tunnels[a].x)<(tunnels[b].r+tunnels[a].r)*(tunnels[b].r+tunnels[a].r)))
                                        tunnels[a].naboy[z]=&tunnels[b];
                                }

                        }

                }


        }

}

void    trace_inittunnels()
{
        tunnels[0].o=0;
        tunnels[0].x=0;
        tunnels[0].y=0;
        tunnels[0].r=-1;

        tunnels[1].o=0;
        tunnels[1].x=0;
        tunnels[1].y=0;
        tunnels[1].r=1.6;

        tunnels[2].o=2;
        tunnels[2].x=5;
        tunnels[2].y=0;
        tunnels[2].r=2.1;

        tunnels[3].o=1;
        tunnels[3].x=5;
        tunnels[3].y=5;
        tunnels[3].r=2.1;

        tunnels[1].o=0;
        tunnels[1].x=5;
        tunnels[1].y=5;
        tunnels[1].r=2.1;

        tunnels[2].o=2;
        tunnels[2].x=5;
        tunnels[2].y=5;
        tunnels[2].r=2.1;

        tunnels[3].o=1;
        tunnels[3].x=5;
        tunnels[3].y=5;
        tunnels[3].r=2.1;

        tunnels[4].o=0;
        tunnels[4].x=5;
        tunnels[4].y=10;
        tunnels[4].r=3.1;

        tunnels[5].o=1;
        tunnels[5].x=10;
        tunnels[5].y=15;
        tunnels[5].r=4.1;

        tunnels[6].o=2;
        tunnels[6].x=15;
        tunnels[6].y=15;
        tunnels[6].r=5.1;

        trace_calculateneighbourinfo();

        { int t; for(t=0;t<ntunnels;t++)tunnels[t].texnum=t*16+1;}

}

static void buildscene(float posx, float posy, float posz)
{
        num=16;

                setcyl(&cyl[0],   0-posx,0-posy,0-posz,  0,1,1, 1, 12);
                setsphere(&s[0],  0-posx,0-posy,0-posz,  2);
                setsphererel(&s[1],&cyl[0],4,2);
                setsphererel(&s[2], &cyl[0],12,  2);

                addsphere(&s[0],&cyl[0]);
             // addsphere(&s[1],&cyl[0]);
                addsphere(&s[2],&cyl[0]);


                setcylrel(&cyl[1],s+2,  0,5,1, 1, 6);
                addsphere(&s[2],&cyl[1]);
                setsphererel(s+3, cyl+1 ,6 ,  2);
                addsphere(&s[3],&cyl[1]);



                setcylrel(&cyl[2],s+3,  1,1,0, 1, 6);
                addsphere(&s[3],&cyl[2]);
                setsphererel(s+4, cyl+2 ,6 ,  2);
                addsphere(&s[4],&cyl[2]);

                setcylrel(&cyl[3],s+3,  -1,1,0, 1, 6);
                addsphere(&s[3],&cyl[3]);
                setsphererel(s+5, cyl+3 ,6 ,  2);
                addsphere(&s[5],&cyl[3]);

                setcylrel(&cyl[4],s+4,  -1,1,0, 1, 6);
                addsphere(&s[4],&cyl[4]);
                setsphererel(s+6, cyl+4 ,6 ,  2);
                addsphere(&s[6],&cyl[4]);


                setcylrel(&cyl[5],s+5,  1,1,0, 1, 6);
                addsphere(&s[5],&cyl[5]);
                addsphere(&s[6],&cyl[5]);


}

static void addsplinepos(float time, int ball)
{
        spline_addkey(&trace_splinex,time,s[ball].x);
        spline_addkey(&trace_spliney,time,s[ball].y);
        spline_addkey(&trace_splinez,time,s[ball].z);
}

void    trace_initsplines()
{
        buildscene(0,0,0);

        spline_init(1000);

        trace_splinex=spline_new();
/*        spline_addkey(&trace_splinex,-10,0);
        spline_addkey(&trace_splinex,10,0);
        spline_addkey(&trace_splinex,20,0);
        spline_addkey(&trace_splinex,50,0);
        spline_addkey(&trace_splinex,70,0);
        spline_addkey(&trace_splinex,4170,0);
*/
        trace_spliney=spline_new();
/*        spline_addkey(&trace_spliney,0,0);
        spline_addkey(&trace_spliney,10,1);
        spline_addkey(&trace_spliney,30,2);
        spline_addkey(&trace_spliney,50,5.5);
        spline_addkey(&trace_spliney,4480,4);
*/
        trace_splinez=spline_new();
/*        spline_addkey(&trace_splinez,0,0);
        spline_addkey(&trace_splinez,10,1);
        spline_addkey(&trace_splinez,30,2);
        spline_addkey(&trace_splinez,50,5.5);
        spline_addkey(&trace_splinez,4480,4);
*/

        addsplinepos(-10,0);
        addsplinepos(70,2);
        addsplinepos(110,3);
        addsplinepos(140,4);
        addsplinepos(180,6);
        addsplinepos(220,5);
        addsplinepos(260,3);
        addsplinepos(300,2);
        addsplinepos(340,0);
}

void    trace_biltabel_init()
{
	int	t,i;
        int     k1,k2;

	for(i=0;i<16;i++)
	for(t=0;t<16;t++)
	{
                k1=i;
                k2=16-i;
                trace_biltabel[i*4+t*64+0]=((k2*(16-t)));
                trace_biltabel[i*4+t*64+1]=(k2*16-((k2*(16-t))));
                trace_biltabel[i*4+t*64+2]=((k1*(16-t)));
                trace_biltabel[i*4+t*64+3]=(k1*16-((k1*(16-t))));
	}
}

int     trace_log(float v)
{
        int     t;

        t=50*log((v+1));

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

int     trace_exp(float t)
{
        int     v;

        v=exp((t-128)/50.0);

        if(v>255)v=255;
        if(v<0)v=0;
        return v;
}

int     trace_log2(float v)
{
        int     t;

        t=128+23*log((v+1));

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

int     trace_exp2(float t)
{
        int     v;

        v=exp((t-256)/23.0);

        if(v>255)v=255;
        if(v<0)v=0;
        return v;
}

void    trace_gouraudtabel_init()
{
        int     t;

        for(t=0;t<512;t++)trace_gouraudtabel[t]=trace_exp(t);
}

void    trace_cycle2(float *a, float *b, float *c)
{
        float temp;

        temp=*c;
        *c=*b;
        *b=*a;
        *a=temp;
}

void    trace_cycle(float *b, float *a, float *c)
{
        float temp;

        temp=*c;
        *c=*b;
        *b=*a;
        *a=temp;
}

void    trace_calculategamma(float x, float y, float z)
{
        int     a;

        for(a=1;a<ntunnels;a++)
        {
                switch(tunnels[a].o)
                {
                        case 0:
                                tunnels[a].gamma=(x-tunnels[a].x)*(x-tunnels[a].x)+(y-tunnels[a].y)*(y-tunnels[a].y)-tunnels[a].r2;
                                break;
                        case 1:
                                tunnels[a].gamma=(y-tunnels[a].x)*(y-tunnels[a].x)+(z-tunnels[a].y)*(z-tunnels[a].y)-tunnels[a].r2;
                                break;
                        case 2:
                                tunnels[a].gamma=(z-tunnels[a].x)*(z-tunnels[a].x)+(x-tunnels[a].y)*(x-tunnels[a].y)-tunnels[a].r2;
                                break;
                }
        }
}

static int insidecyl(cyl2 *cyl, float x,float y,float z)
{
        float dot=(cyl->x-x)*(cyl->u)+(cyl->y-y)*(cyl->v)+(cyl->z-z)*(cyl->w);

        if(-dot<-1)return 0;
        if(-dot>cyl->len) return 0;
        return ((cyl->x-x)*(cyl->x-x)+(cyl->y-y)*(cyl->y-y)+(cyl->z-z)*(cyl->z-z)-dot*dot<(cyl->r*cyl->r));
 return 1;
}

static int lys(float x, float y, float z, float u, float v, float w)
{
        float dot=x*u+y*v+z*w;
        float l=(x*x+y*y+z*z)*(u*u+v*v+w*w);

        return (int)(dot*dot/l*1024);
}

static int lys2(float dx, float dy, float dz, float t, float x, float y, float z, float u, float v, float w)
{
        float gx=dx*t-x;
        float gy=dy*t-y;
        float gz=dz*t-z;

        float dot=(u*gx+v*gy+w*gz)/sqrt(gx*gx+gy*gy+gz*gz);

        float mx=gx-dot*u;
        float my=gy-dot*v;
        float mz=gz-dot*w;

        float l;
        dot=mx*dx+my*dy+mz*dz;

        l=(mx*mx+my*my+mz*mz)*(dx*dx+dy*dy+dz*dz);

        return (int)(dot*dot/l*1024);
}

static trace_p trace_ray2(cyl2 *cyl, float dx, float dy , float dz)
{


      //  float           t;
        int             nohit=10;
        float           hx,hy,hz;

        {
                int     index;
                trace_p screen;
                sphere  *s;
                int f;
                int nohit=10;
                float t,r;
                while(nohit)
               {
                nohit--;

                t=intersectcyl(cyl,dx,dy,dz);
            //    r=cyl->u*(t*dx-cyl->x) +cyl->v*(t*dz-cyl->z)+cyl->w*(t*dz-cyl->z);
                r=cyl->u*(t*dx-cyl->x) +cyl->v*(t*dy-cyl->y)+cyl->w*(t*dz-cyl->z);


                index=(int)r;
                f=1;
                if(index<0)s=cyl->s[0];
                else if(index>=cyl->len)s=cyl->s[cyl->len-1];
                else if((s=cyl->s[index])&&(insidesphere(cyl->s[index],dx*t,dy*t,dz*t)));
                else f=0;
                if(f)
                {
                        if(s)
                        {
                                int i;
                                int c=0;
                                t=intersectsphere(s,dx,dy,dz);
                                for(i=0;i<s->ncyl;i++)
                                if(insidecyl(s->cyl[i],t*dx,t*dy,t*dz))
                                {
                                        cyl=s->cyl[i];
                                        c=1;
                                        break;
                                }
                                if(!c)
                                {
                                        //vi ramte en sf‘re
                                        nohit=0;
                screen.c=trace_lystabel[lys(dx,dy,dz,dx*t-s->x,dy*t-s->y,dz*t-s->z)];
                screen.t=s->num;
                                }

                        }
                        else
                        {
                screen.c=trace_lystabel[lys2(dx,dy,dz,t,cyl->x,cyl->y,cyl->z,cyl->u,cyl->v,cyl->w)];
                screen.t=cyl->num;
                                nohit=0;
                        }

                }
                else
                {
                screen.c=trace_lystabel[lys2(dx,dy,dz,t,cyl->x,cyl->y,cyl->z,cyl->u,cyl->v,cyl->w)];
                screen.t=cyl->num;
                        nohit=0;
                }
               }

//                screen.c=trace_lystabel[(int)((16-t)/8*(1024/2))];
                screen.u=textile*256*256*(dx*t+trace_posx)/4;//atan2(nx,ny)*256*256.0/2/pi;
                screen.v=textile*(dy*t+trace_posy)*256.0*256/32*2*4;
//                screen.t=1;//1+ctunnel<<4;

                return screen;
        }
}

static cyl2* findbesttunnel(cyl2* cyl, int n)
{
        int t;

        for(t=0;t<n;t++)
        if(insidecyl(cyl+t,0,0,0))return cyl+t;


//        printf("x:%8.4f y:%8.4f z:%8.4f u:%8.4f v:%8.4f w:%8.4f\n",cyl->x,cyl->y,cyl->z,cyl->u,cyl->v,cyl->w);

//        exit(1);

        return cyl;
}

void    trace_draw2(int x1, int y1, int x2, int y2, fmatrix m)
{
        float   posx=trace_posx,posy=trace_posy,posz=trace_posz;

        int     xt,yt;
        int     tunnel=1;
        fvertex p[3],q[3];
        fvertex s1,s2;

        cyl2    *bestcyl;

        static float c1;
//        c1+=0.002;

        tunnel=trace_findbesttunnel(posx,posy,posz,m.zu,m.zv,m.zw);

        trace_calculategamma(posx,posy,posz);

        buildscene(posx,posy,posz);


        bestcyl=findbesttunnel(&cyl[0],6);





        q[0].x=-1.0;
        q[0].y=-1.0;
        q[0].z=1.0;
        q[1].x=1.0;
        q[1].y=-1.0;
        q[1].z=1.0;
        q[2].x=-1.0;
        q[2].y=1.0;
        q[2].z=1.0;
        fscalematrixf(&m,1,1,1.5);

        transformf(&m,q,p,3);

        p[1].x=(p[1].x-p[0].x)/80;
        p[1].y=(p[1].y-p[0].y)/80;
        p[1].z=(p[1].z-p[0].z)/80;

        p[2].x=(p[2].x-p[0].x)/50;
        p[2].y=(p[2].y-p[0].y)/50;
        p[2].z=(p[2].z-p[0].z)/50;

        s1=p[0];


        for(yt=y1;yt<=y2;yt++)
        {
                float y=(yt-trace_dimy/2.0)/50.0;

                s2=s1;


                for(xt=x1;xt<=x2;xt++)
                {
                        trace_screen[yt][xt]=trace_ray2(bestcyl,s2.x,s2.y,s2.z);

                        s2.x+=p[1].x;
                        s2.y+=p[1].y;
                        s2.z+=p[1].z;

                }
                s1.x+=p[2].x;
                s1.y+=p[2].y;
                s1.z+=p[2].z;
        }
        cc_timeron(&trace_tim4);

        for(yt=y1;yt<y2;yt++)
        {
                for(xt=x1;xt<x2;xt++)
                {
                        if(!((trace_screen[yt][xt].t==trace_screen[yt][xt+1].t)
                        && (trace_screen[yt][xt].t==trace_screen[yt+1][xt].t)
                        && (trace_screen[yt][xt].t==trace_screen[yt+1][xt+1].t)))
                        {
                                int     xt2,yt2;
                                for(yt2=4*yt;yt2<yt*4+4;yt2++)
                                for(xt2=4*xt;xt2<xt*4+4;xt2++)
                                {
                                        trace_p ts;
                                        float   dx,dy,dz;

                                        dx=p[0].x+(xt2/4.0)*p[1].x+(yt2/4.0)*p[2].x;
                                        dy=p[0].y+(xt2/4.0)*p[1].y+(yt2/4.0)*p[2].y;
                                        dz=p[0].z+(xt2/4.0)*p[1].z+(yt2/4.0)*p[2].z;
//               ts.b=ts.r=ts.g=ts.uu1=ts.u=ts.v=ts.t=ts.uu2=ts.c=0;
                                        ts=trace_ray2(bestcyl,dx,dy,dz);
                                        ((int*)fake)[yt2*320+xt2]=trace_retrgb(&ts);
                                }
                        }
                }
        }

        cc_timeroff(&trace_tim4);
}

float cyl_rot;

static float bolge(float t)
{
        if(t>0)return 0;
        return 10*(0.5-0.5*cos(t*3.1415927*2));
}

void    trace_draw(int precalculate,int useframe)
{
        fmatrix mat;
        static  float   v;

        float voffset;

        sw[SW_CYLOFFSET]+=useframe;
        voffset=bolge(sw[SW_CYLOFFSET]/100.0);

        v+=0.1*2*useframe*1.1;

        resetmatrixf(&mat);

        {
                float tgtx,tgty,tgtz;
                float camx,camy,camz;

                camx=spline_value(trace_splinex,v+voffset);
                camy=spline_value(trace_spliney,v+voffset);
                camz=spline_value(trace_splinez,v+voffset);
                tgtx=spline_value(trace_splinex,v+voffset+2);
                tgty=spline_value(trace_spliney,v+voffset+2);
                tgtz=spline_value(trace_splinez,v+voffset+2);


                tgtx-=camx;
                tgty-=camy;
                tgtz-=camz;

                {
                        static float rot;
                        float y;
                        float z;
                        rot=0.1*cyl_rot+0.9*rot;

                        y=tgty*cos(rot)-tgtz*sin(rot);
                        z=tgty*sin(rot)+tgtz*cos(rot);

                        tgty=y;
                        tgtz=z;
                }

                trace_rotx=atan2(tgty,tgtz);
                trace_roty=-atan2(tgtx,sqrt(tgtz*tgtz+tgty*tgty));

                trace_rotz=0;

                rotatematrixzf(&mat,-trace_rotz);
                rotatematrixxf(&mat,-trace_rotx);
                rotatematrixyf(&mat,-trace_roty);

                trace_posx=spline_value(trace_splinex,v+voffset);
                trace_posy=spline_value(trace_spliney,v+voffset);
                trace_posz=spline_value(trace_splinez,v+voffset);
        }




        cc_timeron(&trace_tim3);
        trace_draw2(0,0,80,50,mat);
        cc_timeroff(&trace_tim3);

}

void trace_init1()
{
        trace_inittunnels();
        trace_initsplines();
        trace_initlystabel();

        cc_addtimer(&trace_tim4,"sub");
        cc_addtimer(&trace_tim3,"trace");
        cc_addtimer(&trace_tim2,"bil");
        cc_addtimer(&trace_tim1,"gridrgb");


//        assert(trace_texture=pcx_read_picture("peder\\sd_tex~1.pcx",0));
//        assert(trace_texture=pcx_read_picture("ftp\\tuntext6.pcx",0));
        assert(trace_texture=pcx_read_picture("ftp\\plastik.pcx",0));
//        assert(trace_texture=pcx_read_picture("\\proj\\textures\\test1.pcx",0));
//        assert(trace_texture=pcx_read_picture("\\3d\\textures\\greyish.pcx",0));

        { int t; for(t=0;t<65536*3;t++)trace_texture[t]=trace_log(trace_texture[t]); }

        trace_gouraudtabel_init();
        trace_biltabel_init();
}

void trace_init2()
{
        trace_uploadtexture(trace_texture);
}

void trace_toscreen2(trace_p s[trace_dimy][trace_dimx], rgb1 *dest)
{
        int     x,y;
        rgb1    col;

        for(y=0;y<trace_dimy-1;y++)
        {
                for(x=0;x<trace_dimx-1;x++)
                {
                        col.r=s[y][x].r;
                        col.g=s[y][x].g;
                        col.b=s[y][x].b;
                        dest[0+0]=col;dest[0+1]=col;dest[0+2]=col;dest[0+3]=col;
                        dest[320+0]=col;dest[320+1]=col;dest[320+2]=col;dest[320+3]=col;
                        dest[640+0]=col;dest[640+1]=col;dest[640+2]=col;dest[640+3]=col;
                        dest[960+0]=col;dest[960+1]=col;dest[960+2]=col;dest[960+3]=col;
                        dest+=4;
                }
                dest+=320*3;
        }
}

layer trace_main(int useframe)
{
        int x,y;
        int r=0,g=0,b=0;
        int fade;

        static  int     c=0;

        if(c<550)
        trace_draw(1,useframe);
        else
        trace_draw(0,useframe);
        cc_timeron(&trace_tim1);
//        trace_calcgridrgb(trace_screen);
        cc_timeroff(&trace_tim1);
        cc_timeron(&trace_tim2);
        trace_toscreen(trace_screen,fake);
        cc_timeroff(&trace_tim2);

        c++;



        fade=sw[SW_CYLFADEVAL];
        return layer_make(fake,0,0,320,200,12,fade*256*256*256+b*65536+g*256+r);
}


void trace_deinit()
{
}
