/*
	TEXTURE HANDLER 0.1b
        echo / fudge
        modifications by craft / fudge
*/

#ifndef TH_C
#define TH_C

#include	<stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        "file.h"
#include        "th.h"
#include        "pcx.h"
#include        "math.h"

void	th_error(char *errmsg)
{
	printf("TH_ERROR: %s\n",errmsg);
	exit(1);
}

void    th_readname(FFILE *f, char *dest)
{
        int     c;

        c=ffgetc(f);
        c=ffgetc(f);

        while(c!='\"')
        {
                *(dest++)=c;
                c=ffgetc(f);
        }
        *dest=0;
}

static int nextchar(FFILE *f)
{
        int k;
        do
        {
                k=ffgetc(f);
        }
        while((k==' ')||(k=='\t'));
        return k;
}

static  void th_readline(FFILE *f, char *a, char *b, int *c)
{
        int k;
        static char number[100];
        char *d;

        th_readname(f,a);

        *b=nextchar(f);
        do
        {
                b++;
                k=*b=ffgetc(f);
        }
        while(!((k==' ')||(k=='\t')));
        *b=0;

        d=number;

        *d=nextchar(f);
        do
        {
                d++;
                k=*d=ffgetc(f);
        }
        while(!((k==' ')||(k=='\t')||(k=='\n')));
        *d=0;

        sscanf(number,"%i",c);
        while((k!='\n')&&(k!=EOF))k=ffgetc(f);

        //printf("%s,%s,%i\n",a,b,c);

        while(((k=='\n')||(k==' ')||(k=='\t'))&&(k!=EOF))k=ffgetc(f);
        if(k!=EOF)fungetc(k,f);
}


void	th_inittexturehandler(th_texturehandler *handler, char *filename)
{
	int	t;
        FFILE    *fp;
        char    tname[20],tpath[128];
	int	ttype;

	//load slotinfo
        fp=ffopen(filename,"r");

	if(fp==0)
	{
		th_error("unable to open texturefile.");
	}

	t=0;
        while(!ffeof(fp))
	{
		t++;
//                th_readname(fp, tname);
//                fscanf(fp," %s %i\n",tpath,&ttype);
                th_readline(fp,tname,tpath,&ttype);
//                fscanf(fp,"%s %s %i\n",tname,tpath,&ttype);
	}

        ffclose(fp);

	handler->slots=(b_texture*)malloc(t*sizeof(b_texture));

	if(handler->slots==0)
	{
		th_error("not enough memory allocating textureslots.");
	}

	handler->numslots=t;

        fp=ffopen(filename,"r");

	for(t=0;t<handler->numslots;t++)
	{
/*                th_readname(fp,handler->slots[t].name);
                fscanf(fp," %s %i\n",  handler->slots[t].path,
				       &handler->slots[t].texturetype);
*/
                th_readline(fp,handler->slots[t].name,handler->slots[t].path,&handler->slots[t].texturetype);
/*                fscanf(fp,"%s %s %i\n",handler->slots[t].name,
				       handler->slots[t].path,
				       &handler->slots[t].texturetype);
*/                handler->slots[t].location=0;
	}

        ffclose(fp);
//        exit(1);
}

int     th_transform(float v)
{
        int     t;

        t=128+(1.0/0.011)*log((v+1)/128);

        if(t>255)t=255;
        if(t<0)t=0;
        return t;
}

void    th_transformtexture(char *tex, int n)
{
        int     t;

        for(t=0;t<n;t++)
        {
                tex[t*4]=th_transform((unsigned char)tex[t*4]);
                tex[t*4+1]=th_transform((unsigned char)tex[t*4+1]);
                tex[t*4+2]=th_transform((unsigned char)tex[t*4+2]);
        }
}

int	th_gettexturenumber(th_texturehandler *handler, char *name)
{
	int	t;
	int	found=-1;

	for(t=0;t<handler->numslots;t++)
	{
		if(!strcmp(handler->slots[t].name,name))
		{
			//texture found
			found=t;
			break;
		}
	}

        if(found==-1)
	{
		//not in table
                return -1; //Rettet af Anders
                //th_error("texture not in handler's list.");
	}

	//determine if texture is loaded

	if(handler->slots[t].location==0)
	{
		pcx_header	hdr;
		char		*dat;

                dat=pcx_read_picture(handler->slots[t].path,1+PCX_ADD2LINES);//<--rettet 11. marts 1999

		pcx_read_header(&hdr,handler->slots[t].path);

		handler->slots[t].width=hdr.xmax-hdr.xmin+1;
                handler->slots[t].height=hdr.ymax-hdr.ymin+1+2;//<--rettet 11. marts 1999

                if((hdr.nplanes==3)&&(hdr.bitsperpixel==8)) //24 bit image
		{
                        th_transformtexture(dat,handler->slots[t].width*handler->slots[t].height);
                        handler->slots[t].location=dat;
		}
                else found=-1; //Rettet af Anders

	}
	return	found;
}



void*	th_gettexturepointer(th_texturehandler *handler, char *name)
{
	int	num;

	num=th_gettexturenumber(handler,name);

	return	handler->slots[num].location;
}



void*	th_gettextureslot(th_texturehandler *handler, char *name)
{
	int	num;

	num=th_gettexturenumber(handler,name);

        if(num==-1) return NULL;
	return	&handler->slots[num];
}

#endif
