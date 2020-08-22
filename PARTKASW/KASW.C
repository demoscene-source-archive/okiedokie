#define scr ((char*)0xa0000)
#define pi 3.14159265358979

#include        <assert.h>
#include        <math.h>

#include        "..\main.h"
#include        "..\scriptde.h"
#include        "..\demolib\layer.h"
#include        "kaswasm.h"
#include        "..\demolib\komms.h"
#include        "..\demolib\vesavbe.h"
//#include        "tinymath.h"
//#include        "bilerp.h"
#include        "..\demolib\col256.h"
//#include        "engine.h"
//#include        "dloader.h"
#include        "..\demolib\convert.h"
#include        "..\demolib\matrix.h"
//#include        "clockcyc\clockcyc.h"
//#include        "spline.h"
//#include        "keyframe.h"
//#include        "quat.h"
//#include        "qspline.h"
#include        "..\demolib\pcx.h"
#include        "..\demolib\lbm.h"

#define kasw_dimx 41
#define kasw_dimy 26
#define radius 35

char    *kasw_texture1;

        static  float rot;
rgb1    kasw_textun[257];

kasw_p kasw_screen[kasw_dimy][kasw_dimx];

char *kasw_tex;
char *kasw_cmap;

float   waterplotx,waterploty,waterplotz;
float   kasw_light=1;

unsigned char kasw_mob[radius*2][radius*2];

int     kasw_fake1[64000];

void    kasw_generatemob()
{
        int     x,y;
        unsigned char *p;

        p=kasw_mob;

        for(y=-radius;y<radius;y++)
        for(x=-radius;x<radius;x++)
        {
                float   l;
                float   l2;

                l=(((float)x*x+(float)y*y)/(radius*radius));

                if(l<1)
                l2=255*(l-1)*(l-1);
                else l2=0;

                *p=l2+0.5;

                p++;
        }
}

void    kasw_plot(float x, float y, rgb1* dest)
{
        int     x1,y1,x2,y2;
        int     i,j;
        int     stepx=0,stepy=0,a,b;

        x1=x-radius;
        y1=y-radius;
        x2=x+radius;
        y2=y+radius;

        if(x1<0){stepx=-x1;x1=0;}
        if(y1<0){stepy=-y1;y1=0;}
        if(x2>320)x2=320;
        if(y2>200)y2=200;


        b=stepy;
        for(j=y1;j<y2;j++)
        {
                a=stepx;
                for(i=x1;i<x2;i++)
                {
                        rgb1    col;
                        float   l;
                        int     l2;

                        l=(((i-x)*(i-x)+(j-y)*(j-y))/(radius*radius));

                        if(l<1)
                        {
                                col=dest[i+j*320];

                                l2=256*(l-1)*(l-1);

                        l2=kasw_mob[b][a];
                                if(col.r+l2<=255)col.r+=l2;else col.r=255;
                                if(col.g+l2<=255)col.g+=l2;else col.g=255;
                                if(col.b+l2<=255)col.b+=l2;else col.b=255;

                                dest[i+j*320]=col;

                        }
                         a++;
                }
                b++;
        }


/*        if((x>=0)&&(x<320)&&(y>=0)&&(y<200))
        {
                dest[x+y*320].r=0;
                dest[x+y*320].g=255;
                dest[x+y*320].b=120;
        }
*/
}

void    kasw_biltabel_init()
{
	int	t,i;
        int     k1,k2;

	for(i=0;i<16;i++)
	for(t=0;t<16;t++)
	{
                k1=i;
                k2=16-i;
                kasw_biltabel[i*4+t*64+0]=((k2*(16-t)));
                kasw_biltabel[i*4+t*64+1]=(k2*16-((k2*(16-t))));
                kasw_biltabel[i*4+t*64+2]=((k1*(16-t)));
                kasw_biltabel[i*4+t*64+3]=(k1*16-((k1*(16-t))));
	}
}

int     kasw_log(float v)
{
        int     t;

        t=50*log((v+1));

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

int     kasw_exp(float t)
{
        int     v;

        v=exp((t-128)/50.0);

        if(v>255)v=255;
        if(v<0)v=0;
        return v;
}

int     kasw_log2(float v)
{
        int     t;

        t=128+23*log((v+1));

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

int     kasw_exp2(float t)
{
        int     v;

        v=exp((t-256)/23.0);

        if(v>255)v=255;
        if(v<0)v=0;
        return v;
}

void    kasw_gouraudtabel_init()
{
        int     t;

        for(t=0;t<512;t++)kasw_gouraudtabel[t]=kasw_exp(t);
}

void    kasw_generatetexture(char *src, rgb1 *dest)
{
        int     angle,t,c;
        float   vinkel;
        float   color;
        int     x,y,dy,dx;
        static  float     xpos=128,ypos=128;
        float   b;
        static  float dist=32;
        float   fr=sw[SW_KASWR]/256.0;
        float   fg=sw[SW_KASWG]/256.0;
        float   fb=sw[SW_KASWB]/256.0;


        xpos=32+dist*cos(rot+=0.07);
        ypos=32+dist*sin(rot);


        for(angle=0;angle<257;angle++)
        {
                vinkel=(float)angle/256*2*pi;

                color=0;

                x=256*xpos;
                y=256*ypos;

                dy=2*256*sin(vinkel);  //2
                dx=2*256*cos(vinkel);

                b=0.3;//1
                for(t=0;t<50;t++)      //50
                {
                        int     mx,my,mxi,myi;
                        int     t1,t2,t3,t4;
                        int     x2,y2;

                        mx=x&255;
                        my=y&255;

                        mxi=256-mx;
                        myi=256-my;

                        x2=255&(x>>8);
                        y2=255&(y>>8);

                        t1=(unsigned char)src[256*(y2&255)+(x2&255)];
                        t2=(unsigned char)src[256*(y2&255)+((x2+1)&255)];
                        t3=(unsigned char)src[256*((y2+1)&255)+(x2&255)];
                        t4=(unsigned char)src[256*((y2+1)&255)+((x2+1)&255)];


                        b=18*2.0/(t+20); //20
                        color+=b*(myi*(t1*mxi+t2*mx)+my*(t3*mxi+t4*mx));
                        x+=dx;
                        y+=dy;
                }


                c=color*(float)1/65536/2;


                c=127+1*(c-127);

                if(c>255)c=255;
                if(c<0)c=0;
//             c=255;

                {
                        float   r;
                        float   g;
                        float   b;

                        r=kasw_cmap[c*3];
                        g=kasw_cmap[c*3+1];
                        b=kasw_cmap[c*3+2];

/*                        r*=kasw_light*(1+0.5*cos(2*pi*angle/256.0));
                        g*=kasw_light*(1+0.4*cos(1+2*pi*angle/256.0));
                        b*=kasw_light*(1+0.5*cos(2+2*pi*angle/256.0));
*/
/*                   r*=kasw_light*(1+0.2*cos(2*pi*angle/256.0));
                        g*=kasw_light*(1+0.12*cos(2*pi*angle/256.0));
                        b*=kasw_light*(1-0.15*cos(1+2*pi*angle/256.0));
*/                   r*=kasw_light*(1+fr*cos(2*pi*angle/256.0));
                        g*=kasw_light*(1+fg*cos(2*pi*angle/256.0));
                        b*=kasw_light*(1+fb*cos(1+2*pi*angle/256.0));


                /*        g=b=r=(r+b+g)/3;
                        r/=4;
                        g/=2;
                  */
                        if(r>255)r=255;if(r<0)r=0;
                        if(g>255)g=255;if(g<0)g=0;
                        if(b>255)b=255;if(b<0)b=0;

                        dest[angle].r=r;
                        dest[angle].g=g;
                        dest[angle].b=b;
                }
        }

}

void    kasw_effekt()
{
        static  float   v;
        int     x,y;
        fvertex ver1[4];
        fvertex ver2[4];
        float   x1,y1,z1;
        float   dx1,dy1,dz1;
        float   x2,y2,z2;
        float   dx2,dy2,dz2;
        float   xpos,ypos,zpos;

        {
                fmatrix mat,mat2;
                static  float   rotx=0,roty=0,rotz=0;
                static  float   rott;

                resetmatrixf(&mat);
/*
                resetmatrixf(&mat2);
//                scalematrixf(&mat,0.5,0.5,1.0);
                rotatematrixxf(&mat,rotx);
                frotatematrixxf(&mat2,-rotx);

                rotatematrixyf(&mat,roty);
                frotatematrixyf(&mat2,-roty);

                rotatematrixzf(&mat,rotz);
                frotatematrixzf(&mat2,-rotz);

                rotatematrixzf(&mat,rot);
                frotatematrixzf(&mat2,rot);
*/
                rotatematrixzf(&mat,rotz+pi/2);
                rotatematrixyf(&mat,roty);//+pi/2);
//                rotatematrixzf(&mat,rotx);
                rotatematrixzf(&mat,rot+pi);


                rott+=1;
                rotx+=0.03;
                roty=0.8+0.6*sin(rott*0.01);
                rotz=0.7*sin(rott*0.005);

                ver1[0].x=-1;ver1[0].y=-1;ver1[0].z=1;
                ver1[1].x= 1;ver1[1].y=-1;ver1[1].z=1;
                ver1[2].x=-1;ver1[2].y= 1;ver1[2].z=1;
                ver1[3].x= 1;ver1[3].y= 1;ver1[3].z=1;

                transformf(&mat,ver1,ver2,4);

                waterplotx=mat2.xw;
                waterploty=mat2.yw;
                waterplotz=mat2.zw;

                waterplotx=mat.zu;  //Matricen er orthonormal
                waterploty=mat.zv;
                waterplotz=mat.zw;

                kasw_light=1+4*mat.zw*mat.zw*mat.zw*mat.zw*mat.zw*mat.zw;
        }

        v+=0.05;

        xpos=0;
        ypos=0;
        zpos=0;

        dx1=(ver2[2].x-ver2[0].x)/25;
        dy1=(ver2[2].y-ver2[0].y)/25;
        dz1=(ver2[2].z-ver2[0].z)/25;

        dx2=(ver2[1].x-ver2[0].x)/40;
        dy2=(ver2[1].y-ver2[0].y)/40;
        dz2=(ver2[1].z-ver2[0].z)/40;

        x1=ver2[0].x;
        y1=ver2[0].y;
        z1=ver2[0].z;

        for(y=0;y<kasw_dimy;y++)
        {
                x2=x1;
                y2=y1;
                z2=z1;

                for(x=0;x<kasw_dimx;x++)
                {
                        float   x1,y1,z1;
                        float   d,t;

                        x1=x2;
                        y1=y2;
                        z1=z2;

                        d=4*(xpos*x1+ypos*y1)*(xpos*x1+ypos*y1)-4*(x1*x1+y1*y1)*(xpos*xpos+ypos*ypos-1000);
                        if(d>=0)
                        {
                                float   x2,y2,z2;
                                double  col;

                                t=(-2*(x1*xpos+y1*ypos)+sqrt(d))/(2*(x1*x1+y1*y1));
                                x2=x1*t+xpos;
                                y2=y1*t+ypos;
                                z2=z1*t+zpos;

                                kasw_screen[y][x].v=(((8*256/2*32.0/pi)*atan2(y2,x2)));
                                kasw_screen[y][x].u=0;//(2*256/2*z2);//+zpos2);
                                if(z1<0)z1*=-1;
                                if(t<0)t*=-1;

                                col=(55-((double)z1*t/4))*256;
                                if((col<456)||(z1*t>500))col=32;

                                kasw_screen[y][x].c=256*128;//col*3;
                        }


                        x2+=dx2;
                        y2+=dy2;
                        z2+=dz2;
                }

                x1+=dx1;
                y1+=dy1;
                z1+=dz1;
        }
}

void    kasw_effekt2()
{
        static  float   v;
        int     x,y;

        v+=0.05;

        for(y=0;y<kasw_dimy;y++)
        for(x=0;x<kasw_dimx;x++)
        {
                kasw_screen[y][x].u=x*256.0*2.0*(1.4+cos(y*0.1+v));
                kasw_screen[y][x].v=y*256.0*2.0*(1.4+cos(x*0.1+v*0.6));
                kasw_screen[y][x].c=256.0*64.0*(1.3+cos((x-y)*0.09+v*2.4)+0.3*sin(y*0.2));

//                kasw_screen[y][x].c=256.0*16*y;

//                kasw_screen[y][x].u=8*256*(x*cos(v)+y*sin(v));
//                kasw_screen[y][x].v=8*256*(x*sin(v)-y*cos(v));
        }
}

void kasw_init1()
{
        kasw_generatemob();
//        assert(kasw_texture1=pcx_read_picture("texture2.pcx"));

//        { int t; for(t=0;t<65536*3;t++)kasw_texture1[t]=kasw_log(kasw_texture1[t]); }

        kasw_tex=lbm_loadilbm("partkasw\\watero3.lbm");
        kasw_cmap=lbm_loadcmap256("partkasw\\watero3.lbm");

//        { int t; for(t=0;t<3*256;t++)printf("%i\n",kasw_cmap[t]);}

        kasw_gouraudtabel_init();
        kasw_biltabel_init();

}

void kasw_init2()
{
        kasw_uploadtexture(kasw_texture1);
}

layer kasw_main(int useframe)
{
        int x,y;
        int r=0,g=0,b=0;
        int fade;

        r=g=b=sw[SW_KASWFLASHCOL];

        kasw_effekt();
//        setborder(34);
        kasw_generatetexture(kasw_tex,kasw_textun);
//        setborder(0);
        kasw_interpolatetexture(kasw_textun,kasw_texture);
        kasw_toscreen2(kasw_screen,kasw_fake1);


        kasw_plot(160*(1.0+waterplotx/waterplotz),100*(1.0+waterploty/waterplotz),(void*)kasw_fake1);

        fade=sw[SW_KASWFADEVAL];
        return layer_make(kasw_fake1,0,0,320,200,12,fade*256*256*256+b*65536+g*256+r);
}


void kasw_deinit()
{
}

