#define scr ((char*)0xa0000)

#include	"math.h"

#include        "..\main.h"
#include        "..\asm.h"
#include        "\demolib\layer.h"
#include        "\demolib\komms.h"
#include        "\demolib\vesavbe.h"
//#include        "tinymath.h"
//#include        "bilerp.h"
#include        "\demolib\col256.h"
//#include        "engine.h"
//#include        "dloader.h"
#include        "\demolib\convert.h"
//#include        "clockcyc\clockcyc.h"
//#include        "spline.h"
//#include        "keyframe.h"
//#include        "quat.h"
//#include        "qspline.h"
//#include        "pcx.h"

#include "\engine\engine.h"
#include "\engine\keyframe.h"
#include "\engine\dloader.h"

rgb1    part_screen[320*200];

rgb1    part_colors[1024];
int     part_o1[200];
int     part_o2[800];

main_videodatamode part_videomode()
{
        return  videodata32;
}

void part_init1()
{
        int     t;


        for(t=0;t<1024;t++)
        {
//        x*256.0*2.0*(1.4+cos(y*0.1+v));
//        y*256.0*2.0*(1.4+cos(x*0.1+v*0.6));
//        256.0*64.0*(1.3+cos((x-y)*0.09+v*2.4)+0.3*sin(y*0.2));
                part_colors[t].r=127+127*cos((2*pi/1024)*t*2+0.6);
                part_colors[t].g=64.0*(1.3+cos((t)*(4*pi/1024))+0.3*sin(t*6*pi/1024));
                part_colors[t].b=127+127*cos((2*pi/1024)*t*6);
/*                part_colors[t].r=127+127*cos(t*0.03);
                part_colors[t].g=127+127*sin(t*0.02);
                part_colors[t].b=127+127*cos(t*0.04);
  */      }

}

void part_init2()
{
}

layer part_main(int useframe)
{
        int x,y;
        rgb1    *fake1;
        static  float   frames=0;

        fake1=part_screen;

        frames+=useframe*0.3;

        {
                int     t;
                int     offset;

                for(t=0;t<800;t++)
                part_o2[t]=256*sin(t*0.006+0.1*frames)+100*cos(t*0.01+0.25*frames)+100*cos(-t*0.009+0.28*frames);
                for(t=0;t<200;t++)
                part_o1[t]=227+127*sin(t*0.005+0.06*frames)+50*cos(t*0.014+0.17*frames)+50*cos(-t*0.015+0.155*frames);
//                part_o1[t]=227+227*sin(t*0.005+0.06*frames);

                offset=15*frames;

                for(y=0;y<200;y++)
                for(x=0;x<320;x++)
                {
                    //    fake1[x+y*320].r=x;
                    //    fake1[x+y*320].g=y;
                        fake1[x+y*320]=part_colors[(y+part_o2[x+part_o1[y]]+offset)&1023];
                }
        }





        return layer_make(part_screen,0,0,320,200,1,127);
}


void part_deinit()
{
}

