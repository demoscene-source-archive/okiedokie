/* by craft / fudge */

#include <stdio.h>
#include <stdlib.h>

#include "lbm.h"
#include "filer.h"

int     lbm_rund(int t)     {return (t+1)&-2;}

void    lbm_til(int j,FFILE *f)
{
        for(;j>0;j--){ffgetc(f);};
}

short   lbm_readword(FFILE *f)
{
        short   j=0;
        j=(j<<8)+ffgetc(f);
        j=(j<<8)+ffgetc(f);
	return j;
}

int   lbm_readlongword(FFILE *f)
{
        int   j=0;
        j=(j<<8)+ffgetc(f);
        j=(j<<8)+ffgetc(f);
        j=(j<<8)+ffgetc(f);
        j=(j<<8)+ffgetc(f);
	return j;
}

char   lbm_readbyte(FFILE *f)
{
        char   j=0;
        j=(j<<8)+ffgetc(f);
	return j;
}

char*   lbm_loadcmap(char* fil)
{
	char *dest;
        FFILE *fp;
	int nj;
        fp=ffopen(fil,"rb");
        if(fp==0){printf("unable to open iff-ilbm file: %s\n",fil);exit(1);};
        lbm_til(12,fp);

	dest=malloc(3*256);
	{	char	a,b,c,d;

                a=ffgetc(fp);
                b=ffgetc(fp);
                c=ffgetc(fp);
                d=ffgetc(fp);
		while(0==(a=='C'&& b=='M' && c=='A' && d=='P'))
		{
			int j;
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=lbm_rund(j);
                        lbm_til(j,fp);
                        a=ffgetc(fp);
                        b=ffgetc(fp);
                        c=ffgetc(fp);
                        d=ffgetc(fp);
		};
	};

        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=lbm_rund(nj);
	{	int	t;
                for(t=0;t<256*3;t++) dest[t]=ffgetc(fp)>>2;
	};

        ffclose(fp);

        return dest;
}

char*   lbm_loadcmap256(char* fil)
{
	char *dest;
        FFILE *fp;
	int nj;
        fp=ffopen(fil,"rb");
        if(fp==0){printf("unable to open iff-ilbm file: %s\n",fil);exit(1);};
        lbm_til(12,fp);

	dest=malloc(3*256);
	{	char	a,b,c,d;

                a=ffgetc(fp);
                b=ffgetc(fp);
                c=ffgetc(fp);
                d=ffgetc(fp);
		while(0==(a=='C'&& b=='M' && c=='A' && d=='P'))
		{
			int j;
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=lbm_rund(j);
                        lbm_til(j,fp);
                        a=ffgetc(fp);
                        b=ffgetc(fp);
                        c=ffgetc(fp);
                        d=ffgetc(fp);
		};
	};

        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=lbm_rund(nj);
	{	int	t;
                for(t=0;t<256*3;t++) dest[t]=ffgetc(fp);//!!!!
	};

        ffclose(fp);

        return dest;
}

char*    lbm_loadilbm(char* fil)
{
	int	p,q;
	char *dest;
        FFILE *fp;
	int nj,ty;
	short bredde,hojde,xoff,yoff,gennem;
	char dybde,mask,komp;
        fp=ffopen(fil,"rb");
        if(fp==0){printf("unable to open iff-ilbm file: %s\n",fil);exit(1);};
        lbm_til(8,fp);
        ty=lbm_readlongword(fp);

        while(0==(ffgetc(fp)=='B' & ffgetc(fp)=='M' & ffgetc(fp)=='H' & ffgetc(fp)=='D'))
	{
		int j;
                j=(j<<8)+ffgetc(fp);
                j=(j<<8)+ffgetc(fp);
                j=(j<<8)+ffgetc(fp);
                j=(j<<8)+ffgetc(fp);
                        j=lbm_rund(j);
                lbm_til(j,fp);
	};

        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=lbm_rund(nj);

        bredde= lbm_readword(fp);nj=nj-2;
        hojde = lbm_readword(fp);nj=nj-2;
        xoff  = lbm_readword(fp);nj=nj-2;
        yoff  = lbm_readword(fp);nj=nj-2;
        dybde = lbm_readbyte(fp);nj=nj-1;
        mask  = lbm_readbyte(fp);nj=nj-1;
        komp  = lbm_readbyte(fp);nj=nj-1;
        lbm_readbyte(fp);nj=nj-1;
        gennem= lbm_readword(fp);nj=nj-2;
        lbm_til(nj,fp);

        if((dest=malloc((int)((bredde+7)&(-8))*hojde))==NULL) exit(1);
	p=0;
	{	char	a,b,c,d;

                a=ffgetc(fp);
                b=ffgetc(fp);
                c=ffgetc(fp);
                d=ffgetc(fp);
		while(0==(a=='B'&& b=='O' && c=='D' && d=='Y'))
		{
			int j;
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=(j<<8)+ffgetc(fp);
                        j=lbm_rund(j);
                        lbm_til(j,fp);
                        a=ffgetc(fp);
                        b=ffgetc(fp);
                        c=ffgetc(fp);
                        d=ffgetc(fp);
		};
	};

        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
        nj=(nj<<8)+ffgetc(fp);
//        nj=lbm_rund(nj);
        while(nj>0)
	{	char	a,b;
		short	t;
                a=ffgetc(fp);nj--;
		if ((a&128)==128)
                {       b=ffgetc(fp);nj--;
			for (t=(short)257-((short)a&0x00ff);t>0;t--)
			{	dest[p]=b;
				p++;
			};
		};

		if (!((a&128)==128))
		{	for (t=((short)a&0x00ff)+1;t>0;t--)
                        {       b=ffgetc(fp);nj--;
				dest[p]=b;
				p++;
			};
		};
	};

	    // p:source
	    // q:dest
	if (ty==((73*256+76)*256+66)*256+77)
	{
                p=q=0;
                {       short   x,y;
                        char    *temp;
                        temp=malloc((bredde+7)&-8);
                        for(y=0;y<hojde;y++)
                        {       for (x=0;x<((bredde+7)&-8);x++){temp[x]=dest[p];p++;};
                                for (x=0;x<bredde;x++)
                                {
                                        unsigned char c,d;
                                        d=255;
                                        if ((x&7)==0) c=128;
                                        if ((x&7)==1) c=64;
                                        if ((x&7)==2) c=32;
                                        if ((x&7)==3) c=16;
                                        if ((x&7)==4) c=8;
                                        if ((x&7)==5) c=4;
                                        if ((x&7)==6) c=2;
                                        if ((x&7)==7) c=1;
                                        if ((temp[(x>>3)]&c)==0) d=d-1;
                                        if ((temp[(x>>3)+((bredde+7)>>3)]&c)==0) d=d-2;
                                        if ((temp[(x>>3)+2*((bredde+7)>>3)]&c)==0) d=d-4;
                                        if ((temp[(x>>3)+3*((bredde+7)>>3)]&c)==0) d=d-8;
                                        if ((temp[(x>>3)+4*((bredde+7)>>3)]&c)==0) d=d-16;
                                        if ((temp[(x>>3)+5*((bredde+7)>>3)]&c)==0) d=d-32;
                                        if ((temp[(x>>3)+6*((bredde+7)>>3)]&c)==0) d=d-64;
                                        if ((temp[(x>>3)+7*((bredde+7)>>3)]&c)==0) d=d-128;
                                        dest[q]=d;
                                        q++;
                                };
                        };
                        free(temp);
                };
	};
        ffclose(fp);
	return dest;
}
