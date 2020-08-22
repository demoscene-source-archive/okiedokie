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

rgb1    vector_screen[320*200];

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
        printf("TEX %i %i %i %i %i %i %i %i\n",tex[0].r,tex[0].g,tex[0].b,tex[0].o,tex[1].r,tex[1].g,tex[1].b,tex[1].o);
        printf("size of: %i\n",sizeof(rgb1));
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

main_videodatamode vector_videomode()
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
        b_camera        *cam;

void vector_init1()
{
        int     t;

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
        setscreenscale(-160.0/256.0,-100.0/256.0);  //her



        cam=A_b_camera(A_b_scene("partvect\\tex.tex"));
        dloader_settexturehandlerpcx(cam->scene,"tom.pcx",8);
//        dloader_settexturehandler(cam->scene,(void*)loadtexture("tex0.raw"));

        b_insertobject(loadobject("partvect\\sdscene8.3ds","*"),cam->scene->root,1);

        keyframe_loadcameraanim("partvect\\sdscene8.3ds","Camera04",cam);
     //   cputs("test hej thorsten\n");
        keyframe_loadanim("partvect\\sdscene8.3ds",cam->scene->root);

        //cputs("test\n");
}

void vector_init2()
{
        sw[SW_VECSPD]=8000;
        sw[SW_VECACC]=8000;
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


layer vector_main(int useframe)
{
        int x,y;
        rgb1    *fake1;
        static  float   frames=0;
        static  float   speed=1;
        static  float   accel=0;


        setscreenscale(-160.0/256.0,-100.0/256.0);  //her

        if(sw[SW_SPOL]!=0)
        {
                frames=sw[SW_SPOL];
                sw[SW_SPOL]=0;
        }



        fake1=vector_screen;



        if(sw[SW_VECSPD]!=8000)
        {
                speed=((float)sw[SW_VECSPD])/100;
                sw[SW_VECSPD]=8000;
        }
        if(sw[SW_VECACC]!=8000)
        {
                accel=((float)sw[SW_VECACC])/1000;
                sw[SW_VECACC]=8000;
        }

        while(useframe>0)
        {
                frames+=speed;
                speed+=accel;
                useframe--;
        }

        keyframe_animcam(cam,frames);
        keyframe_animobjects(cam->scene->root,frames);

//        memset(fake1,0,64000*4);
     memset32(fake1,255*65536+(int)(0.6*256)*256+(int)(0.1*256),64000*4);
        b_render(cam,fake1,fake1);


/*        for(y=0;y<200;y++)
        for(x=0;x<255;x++)
        {
                fake1[x+y*320].r=x;
                fake1[x+y*320].g=y;
        }
  */
        {
                static int fade;

                fade+=10;
                fade=sw[SW_VECFADEVAL];

        return layer_make(vector_screen,0,0,320,200,12,(fade<<24)+255*(65536+256+1));
        }
}


void vector_deinit()
{
}

