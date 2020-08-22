/**************************************************
*RadialZoomblur by Craft/fudGe (november 1998)    *
*zoomblur_init() initialisering                   *
*zoomblur_set()  (om)definer blandingsforholdet   *
*zoomblur()      blur                             *
**************************************************/

#include        <math.h>
#include        "zoomblur.h"
#include        "zbasm.h"

static int *part_screen;


static void klip(int *x1, int *y1, int *x2, int *y2)
{
        if(*x1<0)*x1=0;
        if(*x1>320)*x1=320;
        if(*y1<0)*y1=0;
        if(*y1>200)*y1=200;
        if(*x2<0)*x2=0;
        if(*x2>320)*x2=320;
        if(*y2<0)*y2=0;
        if(*y2>200)*y2=200;
}


static void     initbiltab(int mix)
{
        int     t,i;
        int     k1,k2;
        float   m;

        m=1-mix/256.0;

        zb_mix=mix;

        for(i=0;i<16;i++)
        for(t=0;t<16;t++)
        {
                k1=i;
                k2=16-i;
                zb_biltab[t*4+i*64+0]=(int)(m*((k2*(16-t))));
                zb_biltab[t*4+i*64+1]=(int)(m*(k2*16-((k2*(16-t)))));
                zb_biltab[t*4+i*64+2]=(int)(m*((k1*(16-t))));
                zb_biltab[t*4+i*64+3]=256-mix-zb_biltab[t*4+i*64+0]-zb_biltab[t*4+i*64+1]-zb_biltab[t*4+i*64+2];
        }
}

static void initytabel()
{
        int t;
        for(t=-28;t<228;t++)
        {
                int y=t;
                if(t<0)y=0;
                if(t>198)y=198;

                zb_ytabel[t&255]=y*320*4;
        }
}


static void maketab(int v)
{
        int     t;

        for(t=0;t<256;t++)
        {
                zb_multab[t]=(t*v)>>8;
                zb_multab[t+256]=(t*(255-v))>>8;
        }
}

static void    upos(int x,float a, float c, int y, int *x1, int *x2)
{
        float p1;

        if(a*a<0.00000001)a=0.0001;
        p1=-c*y/a+x;


        if(a>0)
        {
                *x1=p1;
                *x2=320;
        }
        else
        {
                *x1=0;
                *x2=p1;
        }

        if(*x1<0)*x1=0;
        if(*x2>320)*x2=320;
}

static void    mix1(int *x1, int *x2, int *x3, int *x4)
{
        if(*x3>*x1)*x1=*x3;
        if(*x4<*x2)*x2=*x4;
}
/*
static void    zblurkvad(int cx, int cy, float a, float b, float c, float d, int n)
{
        int y;
        float   dux,dvx,duy,dvy;
        int     m;

        dux=(a+1)*65536;
        dvx=(b)*65536;
        duy=(c)*65536;
        dvy=(d+1)*65536;

        switch(n&3)
        {
                case 0:
                {
                        zb_deltau=-dux;
                        zb_deltav=-dvx;

                        for(y=200-1;y>=0;y--)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,a,c,y-cy,&x1,&x2);
                                upos(cx,b,d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&part_screen[y*320+x1],&part_screen2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&part_screen2[y*320+x1],&part_screen[y*320+x1],m*4);else
                                zb_left(&part_screen[y*320+x2-1],part_screen,x2-x1,(x2-1-cx)*dux+(y-cy)*duy+cx*65536,(x2-1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 1:
                {
                        zb_deltau=dux;
                        zb_deltav=dvx;

                        for(y=200-1;y>=0;y--)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,-a,-c,y-cy,&x1,&x2);
                                upos(cx,b,d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&part_screen[y*320+x1],&part_screen2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&part_screen2[y*320+x1],&part_screen[y*320+x1],m*4);else
                                zb_right(&part_screen[y*320+x1],part_screen,x2-x1,(x1-cx)*dux+(y-cy)*duy+cx*65536,(x1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 3:
                {
                        zb_deltau=-dux;
                        zb_deltav=-dvx;

                        for(y=0;y<200;y++)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,a,c,y-cy,&x1,&x2);
                                upos(cx,-b,-d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&part_screen[y*320+x1],&part_screen2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&part_screen2[y*320+x1],&part_screen[y*320+x1],m*4);else
                                zb_left(&part_screen[y*320+x2-1],part_screen,x2-x1,(x2-1-cx)*dux+(y-cy)*duy+cx*65536,(x2-1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 2:
                {
                        zb_deltau=dux;
                        zb_deltav=dvx;

                        for(y=0;y<200;y++)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,-a,-c,y-cy,&x1,&x2);
                                upos(cx,-b,-d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&part_screen[y*320+x1],&part_screen2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&part_screen2[y*320+x1],&part_screen[y*320+x1],m*4);else
                                zb_right(&part_screen[y*320+x1],part_screen,x2-x1,(x1-cx)*dux+(y-cy)*duy+cx*65536,(x1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;

        }
}

static void box(int x1, int y1, int x2, int y2)
{
        int     x,y;

        for(y=y1;y<y2;y++)
        for(x=x1;x<x2;x++)
        {
              ((int*)part_screen)[y*320+x]=0;
        }
}

void    zblur(int cx, int cy, float a, float b, float c, float d)
{
        box(0,0,320,2);
        box(0,0,2,200);
        box(318,0,320,200);
        box(0,198,320,200);

//        if(c>b)//(a+1)*(d+1)>b*c)
        if((a+1)>-(d+1))
        {
        zblurkvad(cx,cy,a,b,c,d,+4);
        zblurkvad(cx,cy,a,b,c,d,0);
        zblurkvad(cx,cy,a,b,c,d,1);
        zblurkvad(cx,cy,a,b,c,d,2);
        zblurkvad(cx,cy,a,b,c,d,3);
        zblurkvad(cx,cy,a,b,c,d,-4);
        zblurkvad(cx,cy,a,b,c,d,0);
        }
        else
        {
        zblurkvad(cx,cy,a,b,c,d,3+4);
        zblurkvad(cx,cy,a,b,c,d,3);
        zblurkvad(cx,cy,a,b,c,d,2);
        zblurkvad(cx,cy,a,b,c,d,1);
        zblurkvad(cx,cy,a,b,c,d,0);
        zblurkvad(cx,cy,a,b,c,d,3-4);
        zblurkvad(cx,cy,a,b,c,d,3);
        }

}
*/
static int clip(int p)
{
        if(p>255)return 254;
        if(p<0)return 0;
        return p&254;
}

void    zoomblur_init(void)
{
        initbiltab(20);
        initytabel();
}

void    zoomblur_set(int v)
{
        initbiltab(v);
}

void    zoomblur(int *screen, int x, int y, float zoom)
{
        int     x1,y1,x2,y2;
        int     s;
        int     dux,duy,dvx,dvy;
        static float v;

        part_screen=screen;


        v+=0.01;

        {
                float u=0.03*sin(v);

                u=0;
                dux=cos(u)*zoom*65536;
                duy=sin(u)*zoom*65536;
                dvx=-sin(u)*zoom*65536;
                dvy=cos(u)*zoom*65536;
        }

                zb_deltau=dux;
                zb_deltav=dvx;

        x1=x;y1=y;x2=320;y2=200;
        klip(&x1,&y1,&x2,&y2);
        for(s=y1;s<y2;s++)zb_right(&part_screen[s*320+x1],part_screen,x2-x1,(x1-x)*dux+(s-y)*duy+x*65536,(x1-x)*dvx+(s-y)*dvy+y*65536);

        x1=x;y1=0;x2=320;y2=y;
        klip(&x1,&y1,&x2,&y2);
        for(s=y2-1;s>=y1;s--)zb_right(&part_screen[s*320+x1],part_screen,x2-x1,(x1-x)*dux+(s-y)*duy+x*65536,(x1-x)*dvx+(s-y)*dvy+y*65536);


                zb_deltau=-dux;
                zb_deltav=-dvx;

        x1=0;y1=0;x2=x;y2=y;
        klip(&x1,&y1,&x2,&y2);
        for(s=y2-1;s>=y1;s--)zb_left(&part_screen[s*320+x2-1],part_screen,x2-x1,(x2-x-1)*dux+(s-y)*duy+x*65536,(x2-x-1)*dvx+(s-y)*dvy+y*65536);

        x1=0;y1=y;x2=x;y2=200;
        klip(&x1,&y1,&x2,&y2);
        for(s=y1;s<y2;s++)zb_left(&part_screen[s*320+x2-1],part_screen,x2-x1,(x2-x-1)*dux+(s-y)*duy+x*65536,(x2-x-1)*dvx+(s-y)*dvy+y*65536);

}
