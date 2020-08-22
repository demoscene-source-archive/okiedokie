//#define LINK

#define pi 3.141592653589793

#define MUSIC

//#define __main_h

// þ þ
int useframe=0;
#define scr ((char*)0xa0000)


#include        <math.h>

#include        <assert.h>
#include        "scriptde.h"
#include        "demolib\komms.h"
#include        "demolib\vesavbe.h"

#include        "main.h"

#include        "demolib\mdemo.h"

#include        "demolib\filer.h"

#include        "demolib\col256.h"
#include        "demolib\convert.h"

#include        "demolib\script.h"

#include        "partvect\vector.h"
#include        "partkasw\kasw.h"
#include        "partcylt\cyl.h"
#include        "partpuls\puls.h"
#include        "peder\soopa.h"
#include        "partmeta\partmeta.h"

void setborder(char);
#pragma aux setborder =\
 "   pusha            ",\
 "   mov bl,al        ",\
 "   mov dx,3c0h      ",\
 "   mov al,11h+32    ",\
 "   out dx,al        ",\
 "   mov al,bl        ",\
 "   out dx,al        ",\
 "   popa             ",\
parm [al] \
modify exact [esi edi eax ebx ecx edx] nomemory;

void setsmode(unsigned short);
#pragma aux setsmode parm [ax] = "int 10h"


short	*VIDEO; 		//Global video pointer
short	*usevideo;

s_script  *script;
int sw[N_SWITCHES];
int     framenum=0;

rgb1    fake1[64000];

int	grayscale=0;

short	vidbuf[64000];
extern  float biltabel[1024];

main_videomode main_thevideomode;


layer   postext,frametext;

layer   startpic1,startpic2,startpic3;
layer   credpic;
layer   okpic2;

layer   lyspic1,lyspic2,lyspic3;

char    THESTRING[256];


void    pts(void)
{
        static int      flip=0;
        static  int     oldframenum=-1;

        if(oldframenum!=framenum)
        {
                oldframenum=framenum;
                if(!grayscale)
                {
                        VBE_SetDisplayStart(0,flip*200);

        //                flip=1-flip;
                        flip=(flip+1)%3;

                        switch(main_thevideomode)
                        {
                                case videomode15:
                                        usevideo=&VIDEO[flip*320*200];
                                        break;
                                case videomode24:
                                        usevideo=&VIDEO[flip*320*100*3];
                                        break;
                                case videomode32:
                                        usevideo=&VIDEO[flip*320*200*2];
                                        break;
                        }
                }
                else
                {
                        usevideo=vidbuf;

                        to256col(vidbuf,scr);

                }
        }
}

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

void    detectscreenmode(char *arg)
{
	int			mode;

        grayscale=1;
        main_thevideomode=videomode15;

        mode=VBE_FindMode(320,200,15);
        if(!((mode==-1)||(!VBE_IsModeLinear(mode))))
        {
                main_thevideomode=videomode15;
                grayscale=0;
        }

        mode=VBE_FindMode(320,200,24);
        if(!((mode==-1)||(!VBE_IsModeLinear(mode))))
        {
                main_thevideomode=videomode24;
                grayscale=0;
        }

        mode=VBE_FindMode(320,200,32);
        if(!((mode==-1)||(!VBE_IsModeLinear(mode))))
        {
                main_thevideomode=videomode32;
                grayscale=0;
        }

        if(!strcmp(arg,"-8"))
        {
                grayscale=1;
                main_thevideomode=videomode15;
        }
        if(!strcmp(arg,"-15"))
        {
                grayscale=0;
                main_thevideomode=videomode15;
        }
        if(!strcmp(arg,"-24"))
        {
                grayscale=0;
                main_thevideomode=videomode24;
        }
        if(!strcmp(arg,"-32"))
        {
                grayscale=0;
                main_thevideomode=videomode32;
        }

        if(grayscale)
        {
                cputs("\n\r- Error initializing VESA videomode\n\r  Try loading UNIVBE 5.3 or newer.\n\r  Use option -8 -15 -24 -32 to force a videomode\n\r Press any key to continue.\n\r");
                getch();
        }




        if(grayscale)
        {
                cputs("\n\r- Running 8 bit videomode\n\r");
        }
        else
        {
                switch(main_thevideomode)
                {
                        case videomode15:
                                cputs("\n\r- Running 15 bit videomode\n\r");
                                break;
                        case videomode24:
                                cputs("\n\r- Running 24 bit videomode\n\r");
                                break;
                        case videomode32:
                                cputs("\n\r- Running 32 bit videomode\n\r");
                                break;
                }
        }
/*
        if(!strcmp(argv[1],"skal_du_med_over_i_f›tex_anders_?"))
	{
                int     t;
		grayscale=1;

                printf("ja\n");

		setsmode(0x13);
		vret();

		for(t=0;t<=93;t++)
		{
			rgb(t,t/1.5,t/1.5,t/1.5);
		}
	}
*/
}

void    setscreenmode(void)
{
	int			mode;
	VBE_ModeInfoBlock	ModeInfo;

        if(grayscale)
        {
                int     t;

                setsmode(0x13);
                vret();                                            //!!

                for(t=0;t<=93;t++)
                {
                        rgb(t,t/1.5,(t+1)/1.5,(t+2)/1.5);
                }
        }
        else
        {
                if(main_thevideomode==videomode15)
                {
                        mode=VBE_FindMode(320,200,15);
                }
                if(main_thevideomode==videomode24)
                {
                        mode=VBE_FindMode(320,200,24);
                }
                if(main_thevideomode==videomode32)
                {
                        mode=VBE_FindMode(320,200,32);
                }
                if(mode==-1)
                {
                        cputs("\n\r Unable to initialize selected video mode!\n\r");
                        exit(1);
                }
                VBE_SetMode(mode,1,1);
                VIDEO=(short*)VBE_GetVideoPtr(mode);

                VBE_Mode_Information(mode,&ModeInfo);
        }
	pts();		//dummy flip for at f† usevideo sat
}

void    settextmode(void)
{
	if(grayscale)
	{
		setsmode(0x03);
	}
	else
	{
		VBE_SetMode(3,0,1);

	}
}

int     interpret()
{
        static  int index=0;

#ifdef MUSIC
        M_updateinfo();
#endif

        while((position>script[index].pos)||((position==script[index].pos)&&(row>=script[index].row)))
        {
                if(script[index].swi==-1)return 1;

                if(script[index].val==(-(1<<31)))
                {
                        sw[script[index].swi]=framenum;
                }
                else
                sw[script[index].swi]=script[index].val;

                index++;
        }
        return 0;
}

layer   layerfade(layer l, int swit)
{
        int  t=framenum-sw[swit];

        if(t<0)l.mix.r=255;           //set f.eks. sizey=0
        else
        if(t>70)l.mix.r=0;           //set f.eks. sizey=0
        else
        l.mix.r=127*cos(t*(1.0/70.0*pi))+127;

        return l;
}

layer   layerfade1(layer l, int swit)
{
        int  t=framenum-sw[swit];
        float  v;

        l.mix.r=255;//s›rg for at billedet faktisk bliver lyst op

        if(t<0)v=0;           //set f.eks. sizey=0
        else
        if(t>70)v=1;
        else
        v=0.5*(1.0-cos(t*(1.0/70.0*pi)));

        v*=255;

        if(l.mix.r>v)l.mix.r=v;

        return l;
}

layer   layerfade0(layer l, int swit)
{
        int  t=framenum-sw[swit];
        float  v;

        if(t<0)v=1;           //set f.eks. sizey=0
        else
        if(t>70)v=0;           //set f.eks. sizey=0
        else
        v=0.5*(1.0+cos(t*(1.0/70.0*pi)));

        v*=255;

        if(l.mix.r>v)l.mix.r=v;

        return l;
}

void    fader(int val, int end, int spd)
{
        sw[val]+=sw[spd]*useframe;

        if(sw[spd]>0)
        {
                if(sw[val]>sw[end])sw[val]=sw[end];
        }
        if(sw[spd]<0)
        {
                if(sw[val]<sw[end])sw[val]=sw[end];
        }
}

void    main(int argc, char *argv[])
{
        extern void *engine_phongmap;
        int     frames=0;
        float     olduf=0;
        char    key=0;

	int			x,y,t;
        int                     debug=0;

	int			intro_exit=0;

        char    *filename;
        char    *cameraname;
        float   speed;
        int     fcold;
        layer   sepic;
        layer   sepic2;

//        if(!malloc(32*1024*1024))exit(1);


#ifdef LINK
        lnk_init("DEMO.EXE",1); //l‘s fra demo.exe
#else
        lnk_init("HARDCOPY.DAT",0); //l‘s fra rigtige filer
#endif

//        lnk_init("HARDCOPY.DAT",1); //l‘s fra datafilen

        script=s_load("script",1000);

#ifdef MUSIC
        M_init1(1);
#endif


        sepic=layer_loadimage("seyoua~2.pcx");
        sepic.mix.type=12;
        sepic.mix.r=0;
        sepic.y=-20;

        sepic2=layer_loadimage("se_log~4.pcx");
        sepic2.mix.type=12;
        sepic2.mix.r=0;
        sepic2.y=0;

        fpupre();

        inttab_init10();

	//INTRO MESSAGE
	//-------------

        //
        //

        layer_initasm();
        layer_init();
        layer_reset();
        layer_setfont(pcx_read_picture("font.pcx",1));

        postext=layer_maketext(0,0,320);
        frametext=layer_maketext(0,8,320);



        engine_phongmap=pcx_read_picture("phong4.pcx",1);


        startpic1=layer_loadimageandmask("peder\\sdl2.pcx","peder\\sdl_a2.pcx");
        startpic1.mix.type=9;
        startpic2=layer_loadimageandmask("peder\\s1_bg.pcx","peder\\s1_bga.pcx");
        startpic2.mix.type=9;
/*        startpic2=layer_loadimage("peder\\s1_bg.pcx");
        startpic2.mix.type=1;
        startpic2.mix.r=128;
*/
        startpic3=layer_loadimage("ftp\\bgfade.pcx");
        startpic3.mix.type=1;
        startpic3.mix.r=128;

        credpic=layer_loadimageandmask("peder\\creds_t3.pcx","peder\\creds_~3.pcx");
        credpic.mix.type=11;
        credpic.mix.r=128;
        credpic.x=38;
        credpic.y=32;


        lyspic1=layer_loadimageandmask("ftp\\lys1.pcx","ftp\\lys1a.pcx");
        lyspic1.mix.type=9;
        lyspic2=layer_loadimageandmask("ftp\\lys2.pcx","ftp\\lys2a.pcx");
        lyspic2.mix.type=9;
        lyspic3=layer_loadimage("ftp\\lys3.pcx");
        lyspic3.mix.type=1;
        lyspic3.mix.r=128;
        lyspic2.x=192+8;
        lyspic3.x=256+8;


        okpic2=layer_loadimage("ftp\\ok2.pcx");
        okpic2.mix.type=1;
        okpic2.mix.r=128;


                //VESA 2.0 INIT
		//-------------

                VBE_Init();
        detectscreenmode(argv[1]);
        setscreenmode();


        settextmode();

        //SETUP
	//-----

        meta_init1();
        meta_init2();

        vector_init1();
        vector_init2();

        kasw_init1();
        kasw_init2();

        trace_init1();
        trace_init2();

        puls_init1();
        puls_init2();

        //part_init1();
        //part_init2();

        wire_init1();
        wire_init2();

        weknows_init1();
        weknows_init2();

        blob_init1();
        blob_init2();

        //MAINLOOP
        //--------

#ifdef MUSIC
        M_init2("mlb_tour.xm");
#endif
                        intro_exit=interpret();
#ifdef MUSIC
        M_setposition(sw[SW_START]);
#endif
        position=sw[SW_START];

        setscreenmode();
        fcold=frameCount;
        {
                int     key=0;

                while((key!=27)&&(!intro_exit))
                {

                        olduf=0.95*olduf+0.05*useframe;

//#ifdef MUSIC
//                        M_waitframe();
//#endif
#ifndef MUSIC
                        frameCount+=3;
#endif

                        intro_exit=interpret();

                        layer_screen=fake1;

                        if(debug)
                        {
                                sprintf(THESTRING,"Position:%3i Pattern:%3i Row:%3i  %02i:%02i",position, pattern, row, position,row);
                                layer_add(layer_printf(postext,THESTRING));
                                sprintf(THESTRING,"Useframe:%i ... %5.2f",useframe,olduf);
                                layer_add(layer_printf(frametext,THESTRING));
                        }

                        if(sw[SW_SPIC1])
                        {
                                static int xpos1=-480;
                                static int xpos2=320;
                                static int bgpos=-640-1000+80;

                                xpos1+=useframe*2;
                                xpos2-=useframe*2;
                                bgpos+=useframe*4;

                                startpic1.x=xpos1;
                                startpic2.x=xpos2;
                                startpic1.y=60;
                                startpic3.x=bgpos;

                                layer_add(startpic1);
                                layer_add(startpic2);
                                layer_add(startpic3);

                                layer_add(layer_make(NULL,bgpos-320-1000,0,320+1000,200,0,(*((int*)startpic3.source))));
                                layer_add(layer_make(NULL,bgpos+640,0,1320,200,0,*((int*)startpic2.source)));
                        }


                        if(sw[SW_CREDON])
                        {
                                fader(SW_CREDVAL,SW_CREDEND,SW_CREDSPD);
                                credpic.mix.r=sw[SW_CREDVAL];
                                layer_add(credpic);
                        }


                        if(sw[SW_WEKNOWS])layer_add(weknows_main(useframe));
                        if(sw[SW_BLOBGFX])
                        {
                                layer_add(lyspic1);
                                layer_add(lyspic2);
                                layer_add(lyspic3);
                        }
                        if(sw[SW_BLOB])
                        {

                                fader(SW_BLOBFADEVAL,SW_BLOBFADEEND,SW_BLOBFADESPD);
                                layer_add(blob_main(useframe));
                        }

                        if(sw[SW_WIRE])layer_add(wire_main(useframe));

//                        if(sw[SW_PART])layer_add(part_main(useframe));

                        if(sw[SW_META])
                        {
                                fader(SW_METAFADEVAL,SW_METAFADEEND,SW_METAFADESPD);
                                layer_add(meta_main(useframe));
                        }

                        if(sw[SW_PULS])layer_add(puls_main(useframe));

                        if(sw[SW_CYLT])layer_add(trace_main(useframe));

                        if(sw[SW_KASW])
                        {

                                fader(SW_KASWFADEVAL,SW_KASWFADEEND,SW_KASWFADESPD);
                                layer_add(kasw_main(useframe));
                        }

                        if(sw[SW_OK])
                        {
                                fader(SW_OKFADEVAL,SW_OKFADEEND,SW_OKFADESPD);
                                okpic2.mix.type=12;
                                okpic2.mix.r=(((int)sw[SW_OKFADEVAL])<<24)+0x010101*((int)sw[SW_OKFADECOL]);
                                layer_add(okpic2);
                        }

                        if(sw[SW_VEC1])
                        {
                                fader(SW_VECFADEVAL,SW_VECFADEEND,SW_VECFADESPD);
                                layer_add(vector_main(useframe));
                        }


                        if(sw[SW_BLANK])layer_add(layer_make(NULL,0,0,320,200,0,0));


                        switch(main_thevideomode)
                        {
                                case videomode15:
                                                        layer_draw15(usevideo);
                                                        break;
                                case videomode24:
                                                        layer_draw24(usevideo);
                                                        break;
                                case videomode32:
                                                        layer_draw32(usevideo);
                                                        break;
                        }

                        frames++;


                        if(kbhit())
                        {

                                key=getch();

                                switch(key)
                                {

#ifdef MUSIC
                                        case '+':       M_setposition(position+1);
                                                        break;
#endif
                                        case '0':       debug=1-debug;
                                                        break;
                                        case 'd':
                                                        {
                                                                FILE *dump;
                                                                static int n;
                                                                static char filename[20];
                                                                int t;
                                                                static char head[]=
                                                                #include "pcxhead.inc"
                                                                sprintf(filename,"dump%i.bmp",n);
                                                                n++;
                                                                assert(dump=fopen(filename,"wb"));
                                                                fwrite(head,54,1,dump);
                                                                for(t=0;t<64000;t++)
                                                                fwrite(((char*)usevideo)+(199-(t/320))*320*4+(t%320)*4,1,3,dump);
                                                                fclose(dump);
                                                        }
                                                        break;
                                }
                        }


                        {
                                int     t;
                                t=frameCount;
                                useframe=t-fcold;
                                fcold=t;
                        }
                        framenum+=useframe;
                        pts();
                }

        }


        pts();
        while(kbhit());

        while(!kbhit())
        {
                static int frame;

                frame+=useframe*3;

                layer_screen=fake1;


                if(frame>256*6)break;

                if(frame<256*3)
                {
                        if(frame<256)
                        {
                                sepic.mix.r=256*256*256*frame+0;
                        }
                        else if(frame<512)
                        {
                                sepic.mix.r=256*256*256*255+0;
                        }
                        else
                        {
                                sepic.mix.r=256*256*256*(3*256-frame)+0xffffff;
                        }
                        layer_add(sepic);
                }
                else
                {
                        if(frame<256*4)
                        {
                                sepic2.mix.r=256*256*256*(frame-3*256)+0xffffff;
                        }
                        else if(frame<256*5)
                        {
                                sepic2.mix.r=256*256*256*255+0;
                        }
                        else
                        {
                                sepic2.mix.r=256*256*256*(6*256-frame)+0;
                        }
                        layer_add(sepic2);
                }

                switch(main_thevideomode)
                {
                        case videomode15:
                                                layer_draw15(usevideo);
                                                break;
                        case videomode24:
                                                layer_draw24(usevideo);
                                                break;
                        case videomode32:
                                                layer_draw32(usevideo);
                                                break;
                }
                {
                        int     t;
                        t=frameCount;
                        useframe=t-fcold;
                        fcold=t;
                }
                framenum+=useframe;
                pts();
        }
        settextmode();
        VBE_Done();

        weknows_deinit();

        blob_deinit();

        wire_deinit();

        puls_deinit();

        kasw_deinit();

        trace_deinit();

//        part_deinit();

        meta_deinit();

	//VESA 2.0 DEINIT
	//---------------


#ifdef MUSIC
        M_deinit();
#endif

        lnk_deinit();
//        lnk_dir();







        printf("soopa doopa - april 1999\n\ncode:   general failure\n        druckluft\ndesign: gibson\n        tough ed\nmusic:  falcon\nenvmap: the cake\npr:     sheriff\n");
}


