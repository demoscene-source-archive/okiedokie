#define dimx 40
#define dimy 40
#define dimz 40

#define maxfaces 2*20000
#define maxvertices 2*20000
#define NBALLS 2


#define scr ((char*)0xa0000)

#include	"math.h"

#include        "..\scriptde.h"
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

#include "meta.h"

#include        "fader.h"

extern    b_texture dl_stdtexture;
extern th_texturehandler *dl_th;

static fvertex fx_verlist[maxvertices],fx_verdestlist[maxvertices];
static fvertex fx_norlist[maxvertices],fx_nordestlist[maxvertices];
static b_face  *fx_facelist;
static b_object *fx_obj;
static metaball balls3[NBALLS];

static rgb1    part_screen[320*200];

static rgb1     *background;

static int startx,starty,startz;


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


static rgb1    *dummytexture(void)
{
        static  rgb1    *p;
        int             x,y;

        if(p)return p;

        p=malloc(256*256*sizeof(rgb1));

        for(y=0;y<255;y++)
        for(x=0;x<255;x++)
        {
                p[256*y+x].r=(2*x)&128;
                p[256*y+x].g=(2*y)&128;
                p[256*y+x].b=128;
                p[256*y+x].o=128;

        }
        return p;
}


static                b_camera        *cam;
static                b_object        *obj4;
void meta_init1()
{
        char filename[]="partmeta\\test3.3ds";
        char cameraname[]="Camera05"; //2

        fade_init();


        background=pcx_read_picture("mads\\hej.pcx",1);




                cam=A_b_camera(A_b_scene("partmeta\\tex.tex"));
                dloader_settexturehandler(cam->scene,(void*)dummytexture());

                b_insertobject(obj4=loadobject(filename,"*"),cam->scene->root,1);

                keyframe_loadcameraanim(filename,cameraname,cam);
                keyframe_loadanim(filename,cam->scene->root);


        fx_facelist=A_b_facelist(maxfaces);
        {int t;b_texture *p=th_gettextureslot(dl_th,"phong");for(t=0;t<maxfaces;t++)fx_facelist[t].texture=p;}
        {int t;b_texture *p=th_gettextureslot(dl_th,"phong");for(t=0;t<maxfaces;t++)fx_facelist[t].maptype=p->texturetype;}


        fx_obj=A_b_object("Meta",fx_facelist,0,fx_verlist,0,fx_verdestlist);
        fx_obj->norlist=fx_norlist;
        fx_obj->nordestlist=fx_nordestlist;
        dl_stdtexture.texturetype=4;

        fx_obj->x=-15*3*32;
        fx_obj->y=-15*3*16;
        fx_obj->z=-15*3*16;


        cam->scene->root->next->on=0;
        cam->scene->root->next->next->on=0;

        b_insertobject(fx_obj,cam->scene->root,1);

//        fx_obj->realtimenormals=1;

        fx_obj->rotationtype=b_eulerrot;
        {
                void    *old=NULL;
                int     t;

                for(t=NBALLS-1;t>=0;t--)
                {
                        balls3[t].next1=old;
                        old=&balls3[t];

                        balls3[t].size0=5;
                        balls3[t].size=balls3[t].size0*balls3[t].size0;
                        balls3[t].ssinv=1.0/balls3[t].size;

                        balls3[t].t1=0;
                        balls3[t].t2=(t+1)/150.00*10*2/2;
                }
        }

}

void meta_init2()
{
        setscreenscale(-160.0/256.0,-100.0/256.0);
}


static void updatestart(float x, float y, float z)
{
        if(startx>x-6)startx=x-6;
        if(starty>y-6)starty=y-6;
        if(startz>z-6)startz=z-6;
        if(startx+dimx<x+6)startx=x+6-dimx;
        if(starty+dimy<y+6)starty=y+6-dimy;
        if(startz+dimz<z+6)startz=z+6-dimz;
}


static float x3=10,y3=10,z3=10;
static float x2=10,y2=10,z2=10;


void kryds(float a, float b, float c, float x, float y, float z, float *u, float *v, float *w)
{
        *u=b*z-c*y;
        *v=c*x-a*z;  //?
        *w=a*y-b*x;
}

void calcpos(float x, float y, float z, float twist)
{
        static float oldx,oldy,oldz;

        float vx=x-oldx;
        float vy=y-oldy;
        float vz=z-oldz;

        float ox,oy,oz,l;
        float ox2,oy2,oz2,l2;
        float s=sin(twist);
        float c=cos(twist);


        kryds(1,0,0,vx,vy,vz,&ox,&oy,&oz);

        l=3.0/sqrt(ox*ox+oy*oy+oz*oz);
        ox*=l;
        oy*=l;
        oz*=l;

        kryds(ox,oy,oz,vx,vy,vz,&ox2,&oy2,&oz2);

        l2=3.0/sqrt(ox2*ox2+oy2*oy2+oz2*oz2);
        ox2*=l2;
        oy2*=l2;
        oz2*=l2;

        x3=x+c*ox+s*ox2;
        y3=y+c*oy+s*oy2;
        z3=z+c*oz+s*oz2;
        x2=x-c*ox-s*ox2;
        y2=y-c*oy-s*oy2;
        z2=z-c*oz-s*oz2;

        oldx=x;
        oldy=y;
        oldz=z;
}



layer meta_main(int useframe)
{
        int x,y;
        rgb1    *fake1;
        static  float   frames=0;

        static float ind;
        static float ud;


        float v=1;
        int     t;

        for(t=0;t<useframe;t++)v*=0.96;

        if(sw[SW_METAUD])ud+=useframe;


        ind+=useframe;
        if(ind>30)ind=30;



        fake1=part_screen;

        frames+=useframe*0.3;

        frames+=sw[SW_METASPD]*0.0005*useframe;
        sw[SW_METASPD]-=7*useframe;
        if(sw[SW_METASPD]<0)sw[SW_METASPD]=0;




//                        fx_obj->rotx+=0.1;

                        {
                                int     t;

                                for(t=NBALLS-1;t>=0;t--)
                                {
                            //            balls3[t].t1+=balls3[t].t2;
                            //            balls3[t].x=16+5*sin(balls3[t].t1);
                            //            balls3[t].y=16+5*cos(3*balls3[t].t1);
                            //            balls3[t].z=16+5*sin(4*balls3[t].t1);
                                        balls3[t].t1+=balls3[t].t2;
                                        balls3[t].x=32+20*sin(balls3[t].t1);
                                        balls3[t].y=16+3.5*cos(2*sin(3*balls3[t].t1));
                                        balls3[t].z=16+3.5*sin(2*sin(3*balls3[t].t1));
                                }
                                        balls3[1].x=balls3[0].x;
                                        balls3[1].y=32-balls3[0].y;
                                        balls3[1].z=32-balls3[0].z;


        if(useframe!=0)
                        calcpos(20+10*cos(frames*0.2)+42-0.09*(ind-30)*(ind-30)+0.006*ud*ud,20+10*sin(frames*0.1),20+7*sin(frames*0.08),2*sin(frames*0.2));

                        balls3[0].x=x3;
                        balls3[0].y=y3;
                        balls3[0].z=z3;
                        balls3[1].x=x2;
                        balls3[1].y=y2;
                        balls3[1].z=z2;


                        for(t=0;t<2;t++)
                        updatestart(balls3[t].x,balls3[t].y,balls3[t].z);

//                        startx=starty=startz=0;
//                        startx=1;
//                        printf("%i %i %i\n",startx,starty,startz);
                        }

                        meta(fx_facelist,fx_verlist,fx_verdestlist,fx_norlist,fx_nordestlist,balls3,startx,starty,startz,v);

                        fx_obj->numfaces=meta_nf;
                        fx_obj->numvertices=meta_vertexnum;
                        fx_obj->numnormals=meta_vertexnum;



                        keyframe_animcam(cam,frames*1);
                        keyframe_animobjects(cam->scene->root,frames*1);

                        cam->tgtx+=0*4*8*20*sin(balls3[0].t1);

                        resetmatrixf(&fx_obj->mat);

//     memset32(fake1,(int)(0.22*255)*65536+(int)(0.16*255)*256+(int)(0.22*255),64000*4);

              //          memcpy(fake1,background,64000*4);
              fade(background,fake1,64000,sw[SW_METAFADEVAL],255*65536+(int)(0.6*256)*256+(int)(0.1*256));
                        b_render(cam,fake1,fake1);




        return layer_make(part_screen,0,0,320,200,1,127);
}


void meta_deinit()
{
}

