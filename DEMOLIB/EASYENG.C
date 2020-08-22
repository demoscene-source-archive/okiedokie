#include "easyeng.h"

static void    inttab_init10()
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

static int     transform(float v)
{
        int     t;

        t=128+(1.0/0.011)*log((v+1)/128);

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

static int      transformexp(int t)
{
        int     v;

        v=128*exp((t-256)*0.011);

        if(v>255)return 255;
        else if(v<0)return 0;
        else return v;

        return v;
}

static  void    initgouraudtabel()
{
        int     t;
        for(t=0;t<512;t++) gouraudtabel[t]=transformexp(t);
}

static rgb1    *transformtex(rgb1 *tex)
{
        int     t;

        for(t=0;t<65536;t++)
        {
                tex[t].r=transform((unsigned char)tex[t].r);
                tex[t].g=transform((unsigned char)tex[t].g);
                tex[t].b=transform((unsigned char)tex[t].b);
        }
        return tex;
}


static char* defaultpcxfile="No default pcx-filename";

static rgb1 *defaulttex(void)
{
        static  rgb1    *default1;

        if(!default1)default1=transformtex(pcx_read_picture(defaultpcxfile,1));
        return default1;
}

static initialized;

void    ee_init(char *pcxfile)
{
        if(!initialized)
        {
                initgouraudtabel();

                b_init();

                inttab_init10();
                setscreenscale(-160.0/256.0,-100.0/256.0);

                defaultpcxfile=pcxfile;

                initialized=1;
        }
}

b_camera *ee_loadscene(char *name3ds, char *namecam, char *nametexfile)
{
        b_camera        *cam;

        if(!namecam)namecam="Camera01";
        if(!nametexfile)nametexfile="tex.tex";

        cam=A_b_camera(A_b_scene(nametexfile));
        dloader_settexturehandler(cam->scene,(void*)defaulttex());

        b_insertobject(loadobject(name3ds,"*"),cam->scene->root,1);

        keyframe_loadcameraanim(name3ds,namecam,cam);
        keyframe_loadanim(name3ds,cam->scene->root);

        return cam;
}

rgb1    *ee_animanddraw(b_camera *cam, float frames, rgb1 *screen)
{
        keyframe_animcam(cam,frames);
        keyframe_animobjects(cam->scene->root,frames);
        b_render(cam,screen,screen);

        return screen;
}

void    ee_deinit()
{
        initialized=0;
}


