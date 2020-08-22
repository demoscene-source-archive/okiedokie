#define scr ((char*)0xa0000)

#include	"math.h"

#include        "..\main.h"
#include        "..\demolib\layer.h"
#include        "..\demolib\komms.h"
#include        "..\demolib\vesavbe.h"
//#include        "tinymath.h"
//#include        "bilerp.h"
#include        "..\demolib\col256.h"
//#include        "engine.h"
//#include        "dloader.h"
#include        "..\demolib\convert.h"
//#include        "clockcyc\clockcyc.h"
//#include        "spline.h"
//#include        "keyframe.h"
//#include        "quat.h"
//#include        "qspline.h"
//#include        "pcx.h"

#include "..\demolib\engine.h"
#include "..\demolib\keyframe.h"
#include "..\demolib\dloader.h"

#define puls_dimx 17
#define puls_dimy 78

extern    b_texture dl_stdtexture;

rgb1    puls_screen[320*200];

float     puls_frames;
static vertex delta;

static rgb1 *background;


extern  float biltabel[1024];
void    inttab_init10()
{
	int	t,i;
        int     k1,k2;

	for(i=0;i<16;i++)
	for(t=0;t<16;t++)
	{
                k1=i;
                k2=16-i;
                biltabel[i*4+t*64+0]=((k2*(16-t)));
                biltabel[i*4+t*64+1]=(k2*16-((k2*(16-t))));
                biltabel[i*4+t*64+2]=((k1*(16-t)));
                biltabel[i*4+t*64+3]=(k1*16-((k1*(16-t))));
	}
}

int     transform(float v)
{
        int     t;

        t=128+(1.0/0.011)*log((v+1)/128);

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

rgb1    *loadtexture(char *filename)
{
        rgb1     *tex;
        FILE    *f;
        f=fopen(filename,"rb");
        if(f==NULL)
        {
                printf("Unable to open texture: %s\n",filename);
                exit(1);
        }
        tex=(void*)malloc(65536*sizeof(rgb1));
        fread(tex,sizeof(rgb1),65536,f);
        //printf("TEX %i %i %i %i %i %i %i %i\n",tex[0].r,tex[0].g,tex[0].b,tex[0].o,tex[1].r,tex[1].g,tex[1].b,tex[1].o);
        //printf("size of: %i\n",sizeof(rgb1));
        fclose(f);


        {
                int     t;

                for(t=0;t<65536;t++)
                {
                        tex[t].r=transform((unsigned char)tex[t].r);
                        tex[t].g=transform((unsigned char)tex[t].g);
                        tex[t].b=transform((unsigned char)tex[t].b);
                }
        }

        return tex;
}

main_videodatamode puls_videomode()
{
/*        switch(main_thevideomode)
        {
                case    videomode15: return videodata24;
                case    videomode24: return videodata32;
                case    videomode32: return videodata32;
        }
*/
        return  videodata32;
}
        b_camera        *puls_cam;
        b_object        *puls_pathobj,*puls_obj;
        keyframe_multispline *puls_spline;
        b_face          puls_facelist[2*puls_dimx*puls_dimy];
        fvertex        puls_verlist[puls_dimx*(puls_dimy+1)];
        fvertex        puls_norlist[puls_dimx*(puls_dimy+1)];
        fvertex        puls_destlist[puls_dimx*(puls_dimy+1)];

void    puls_getpoint(float t, float *x, float *y, float *z)
{
        *x=spline_value(puls_spline->x,t)+delta.x;
        *y=spline_value(puls_spline->y,t)+delta.y;
        *z=spline_value(puls_spline->z,t)+delta.z;

//        *x+=50*sin(puls_frames*0.03 );//+*x*0.0068+*z*0.008);
//        *y+=50*sin(puls_frames*0.031);//+*y*0.0060+*z*0.006);
//        *z+=50*sin(puls_frames*0.035);//+*z*0.0081+*y*0.007);
}

void    puls_getvel(float t, float *x, float *y, float *z)
{
        float   a,b,c,d,e,f,l;

        puls_getpoint(t-0.1,&a,&b,&c);
        puls_getpoint(t+0.1,&d,&e,&f);


        *x=d-a;
        *y=e-b;
        *z=f-c;

        l=1.0/sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));

        *x*=l;
        *y*=l;
        *z*=l;
}

void    puls_getbinormal(float t, float *x, float *y, float *z)
{
        float   a,b,c,d,e,f,l;

        puls_getpoint(t-0.1,&a,&b,&c);
        puls_getpoint(t+0.1,&d,&e,&f);


        *x=b*f-c*e;
        *y=c*d-a*f;
        *z=a*e-b*d;

        l=1.0/sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));

        *x*=l;
        *y*=l;
        *z*=l;
}

void    puls_getnormal(float t, float *x, float *y, float *z)
{
        float   a,b,c,d,e,f,l;

        puls_getvel(t,&a,&b,&c);
        puls_getbinormal(t,&d,&e,&f);

        *x=b*f-c*e;
        *y=c*d-a*f;
        *z=a*e-b*d;

        l=1.0/sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));

        *x*=l;
        *y*=l;
        *z*=l;
}

void    puls_rot(float *a, float *b, float v)
{
        float   x,y;

        x=sin(v)*(*a)+cos(v*0.9)*(*b);
        y=sin(v)*(*b)-cos(v)*(*a);

        *a=x;
        *b=y;
}

void    puls_makevertices(float v)
{
        int     x,y;

        for(y=0;y<=puls_dimy;y++)
        {
                float   px,py,pz;
                float   bx,by,bz;
                float   nx,ny,nz;
                float   r1=sin(y*0.3+v*0.7),r2=cos(y*0.3+v*0.7);

                float   size;
                float   t=v+y*1.2/2;
                int     ti=t/2.0/pi;

                size=1;
                size+=0.1*(-cos(t+v*0.3))-0.15*(-cos(-0.7*t+v*0.2));

                puls_getpoint(0.5*y+1,&px,&py,&pz);
                puls_getbinormal(0.5*y+1,&bx,&by,&bz);
                puls_getnormal(0.5*y+1,&nx,&ny,&nz);

                for(x=0;x<puls_dimx;x++)
                {
                        float s,c;
                        float s2=size*(1.0+0.15*sin(2*pi*x/puls_dimx+0.2*v)+0.15*sin(-2*pi*x/puls_dimx+0.3*v));

                        s2*=s2;
                        s2*=s2;
/*                        s2*=s2;
  */
                        s=100*s2*sin(2*pi*x/puls_dimx+0.5*v);
                        c=100*s2*cos(2*pi*x/puls_dimx+0.5*v);

                        puls_verlist[x+y*puls_dimx].x=px+c*bx+s*nx;
                        puls_verlist[x+y*puls_dimx].y=py+c*by+s*ny;
                        puls_verlist[x+y*puls_dimx].z=pz+c*bz+s*nz;
                }

        }

}
void    puls_makeverticesold(float v)
{
        int     x,y;

        for(y=0;y<=puls_dimy;y++)
        {
                float   px,py,pz;
                float   bx,by,bz;
                float   nx,ny,nz;
                float   r1=sin(y*0.3+v*0.7),r2=cos(y*0.3+v*0.7);

                float   size;
                float   t=v+y*1.2/2;
                int     ti=t/2.0/pi;

                size=100;
                if((ti%1)==0)
                size+=50*(-cos(t)+1);

                puls_getpoint(0.5*y+1,&px,&py,&pz);
                puls_getbinormal(0.5*y+1,&bx,&by,&bz);
                puls_getnormal(0.5*y+1,&nx,&ny,&nz);

/*
                puls_rot(&nx,&bx,y*0.2+v*0.3);
                puls_rot(&ny,&by,y*0.2+v*0.3);
                puls_rot(&nz,&bz,y*0.2+v*0.3);
  */
                for(x=0;x<puls_dimx;x++)
                {
                        float   s=1*size*sin(2*pi*x/puls_dimx);
                        float   c=1*size*cos(2*pi*x/puls_dimx);

                  //      float   s=r1*s1+r2*c1;
                //        float   c=r2*s1-r1*c1;

                        puls_verlist[x+y*puls_dimx].x=px+c*bx+s*nx;
                        puls_verlist[x+y*puls_dimx].y=py+c*by+s*ny;
                        puls_verlist[x+y*puls_dimx].z=pz+c*bz+s*nz;
                }

        }

}
void    puls_makefaces()
{
        int     x,y;
        float   mul;

        mul=1.0/puls_dimx;

        for(y=0;y<puls_dimy;y++)
        {
                for(x=0;x<puls_dimx;x++)
                {
                        puls_facelist[2*(y*puls_dimx+x)]=VAL1_b_face(&puls_destlist[y*puls_dimx+x],&puls_destlist[y*puls_dimx+(x+1)%puls_dimx],&puls_destlist[(y+1)*puls_dimx+x],2,x*mul,(y)*0.1,(x+1)*mul,(y)*0.1,x*mul,(y+1)*0.1,&dl_stdtexture);
                        puls_facelist[2*(y*puls_dimx+x)+1]=VAL1_b_face(&puls_destlist[(y+1)*puls_dimx+(x+1)%puls_dimx],&puls_destlist[(y+1)*puls_dimx+x],&puls_destlist[y*puls_dimx+(x+1)%puls_dimx],2,(x+1)*mul,(y+1)*0.1,x*mul,(y+1)*0.1,(x+1)*mul,(y)*0.1,&dl_stdtexture);
                }

        }

}

void puls_init1()
{
        int     t;


        background=pcx_read_picture("mads\\bagg1b.pcx",1);

        for(t=0;t<512;t++)
        {
                int     v;

                v=128*exp((t-256)*0.011);

                if(v>255)gouraudtabel[t]=255;
                else if(v<0)gouraudtabel[t]=0;
                else gouraudtabel[t]=v;
        }

        b_init();

        inttab_init10();
        setscreenscale(-160.0/256.0,-100.0/256.0);



        puls_cam=A_b_camera(A_b_scene("partpuls\\tex.tex"));
//        dloader_settexturehandler(puls_cam->scene,(void*)loadtexture("tex0.raw"));
//        dloader_settexturehandlerpcx(puls_cam->scene,"\\proj\\textures\\r2.pcx",8);
//        dloader_settexturehandlerpcx(puls_cam->scene,"peder\\sd_tex~1.pcx",8);
//        dloader_settexturehandlerpcx(puls_cam->scene,"\\3d\\textures\\greyish.pcx",8);
//        dloader_settexturehandlerpcx(puls_cam->scene,"ftp\\tuntext2.pcx",8);
//        dloader_settexturehandlerpcx(puls_cam->scene,"..\\textures\\r2.pcx",8);
//        dloader_settexturehandlerpcx(puls_cam->scene,"..\\textures\\test1.pcx",8);
        dloader_settexturehandlerpcx(puls_cam->scene,"ftp\\indi.pcx",8);

        b_insertobject(loadobject("partpuls\\puls7.3ds","*"),puls_cam->scene->root,1);

        keyframe_loadcameraanim("partpuls\\puls7.3ds","Camera02",puls_cam);
        keyframe_loadanim("partpuls\\puls7.3ds",puls_cam->scene->root);

        puls_pathobj=b_findobject(puls_cam->scene->root,"Object01");
        puls_pathobj->on=0;
        puls_spline=&puls_pathobj->posspline;

        puls_obj=A_b_object("Slange",puls_facelist,puls_dimx*puls_dimy*2,puls_verlist,puls_dimx*(puls_dimy+1),puls_destlist);

//        puls_obj->numfaces=20;

//        puls_verlist[0]=VAL_b_fvertex(0,0,0);
//        puls_verlist[1]=VAL_b_fvertex(00,100,0);
//        puls_verlist[2]=VAL_b_fvertex(100,100,0);

//b_face VAL1_b_face(fvertex* a, fvertex* b, fvertex* c, int maptype, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, b_texture* texture)
//        puls_facelist[0]=VAL1_b_face(&puls_destlist[0],&puls_destlist[1],&puls_destlist[2],0,0,2000,2000,0,2000,2000,&dl_stdtexture);

        dl_stdtexture.texturetype=8;//3

        puls_makefaces();

        b_generatetinyfaceinfo(puls_obj);

        b_insertobject(puls_obj,puls_cam->scene->root,1);

        puls_makevertices(0);

        b_initsimplenormals(puls_obj);

        puls_obj->realtimenormals=1;

        //cputs("test\n");

        delta=puls_pathobj->verlist[0];


/*       {
        spline_key *s;
        s=spline_new();
        spline
  */
}

void puls_init2()
{
}


void memset32(void *, int, int);
#pragma aux memset32 =\
   "pusha",\
   "mov ebx,ecx",\
   "shr ecx,2",\
   "shl ecx,2",\
   "sub ebx,ecx",\
   "shr ecx,2",\
   "rep stosd",\
   "mov ecx,ebx",\
   "rep stosb",\
   "popa",\
parm [edi] [eax] [ecx] \
modify exact [esi edi eax ebx ecx edx];

layer puls_main(int useframe)
{
        int x,y;
        rgb1    *fake1;
        static  float   frames=0;

        fake1=puls_screen;

        frames+=0.05*useframe*0.5*0.8;
        puls_frames+=useframe*0.5;
/*
        for(y=0;y<200;y++)
        for(x=0;x<255;x++)
        {
                fake1[x+y*320].r=x;
                fake1[x+y*320].g=y;
                fake1[x+y*320].b=0;
        }
  */
      //  memset(fake1,0,64000*4);
 //       memset32(fake1,(int)(0.5*255)*65536+(int)(0.3*256)*256+(int)(0.1*256),64000*4);
//        memset32(fake1,(int)(0.22*255)*65536+(int)(0.16*256)*256+(int)(0.22*256),64000*4);


                        memcpy(fake1,background,64000*4);

        puls_makevertices(frames*2);

        keyframe_animcam(puls_cam,frames);
        keyframe_animobjects(puls_cam->scene->root,frames);
        b_render(puls_cam,fake1,fake1);



        return layer_make(puls_screen,0,0,320,200,1,127);
}


void puls_deinit()
{
}

