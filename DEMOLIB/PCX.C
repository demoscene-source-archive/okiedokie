/* echo / fudge modified by craft / fudge */
/*
	PCX LOADER 0.1b
*/

#ifndef PCX_C
#define PCX_C

#include	<stdio.h>
#include        <stdlib.h>
#include        <assert.h>
#include        "pcx.h"
#include        "filer.h"

int     pcx_file_getbyte(FFILE *a)
{
        return ((unsigned char)ffgetc(a));
}

void	pcx_error(char *errmsg, char *filename)
{
	printf("PCX_ERROR: %s (%s)\n",errmsg,filename);
	exit(1);
}


int     pcx_file_getword(FFILE *a)
{
	char	b[4];
	b[0]=pcx_file_getbyte(a);
	b[1]=pcx_file_getbyte(a);
	b[2]=0;
	b[3]=0;
	return ((int*)b)[0];
}

void    pcx_file_skipbyte(FFILE *f, int j)
{
        for(;j>0;j--){pcx_file_getbyte(f);};
}


void	pcx_read_header(pcx_header *hdr, char *filename)
{
        FFILE    *fp;

        fp=ffopen(filename,"rb");

	if(fp==0)
	{
		pcx_error("unable to open .pcx-file.",filename);
	}

	hdr->manufacturer=pcx_file_getbyte(fp);
	hdr->version=pcx_file_getbyte(fp);
	hdr->encoding=pcx_file_getbyte(fp);
	hdr->bitsperpixel=pcx_file_getbyte(fp);
	hdr->xmin=pcx_file_getword(fp);
	hdr->ymin=pcx_file_getword(fp);
	hdr->xmax=pcx_file_getword(fp);
	hdr->ymax=pcx_file_getword(fp);

	pcx_file_skipbyte(fp,53);

	hdr->nplanes=pcx_file_getbyte(fp);
	hdr->bytesperline=pcx_file_getword(fp);

        ffclose(fp);
}

void	pcx_print_header(pcx_header *hdr)
{
	printf("Manufacturer: %d\n",hdr->manufacturer);
	printf("Version     : %d\n",hdr->version);
	printf("Encoding    : %d\n",hdr->encoding);
	printf("BitsPerPixel: %d\n",hdr->bitsperpixel);
	printf("Xmin        : %d\n",hdr->xmin);
	printf("Ymin        : %d\n",hdr->ymin);
	printf("Xmax        : %d\n",hdr->xmax);
	printf("Ymax        : %d\n",hdr->ymax);
	printf("Nplanes     : %d\n",hdr->nplanes);
	printf("BytesPerLine: %d\n",hdr->bytesperline);
}

void    pcx_read_data(pcx_header *hdr, char *filename, char *pic, int skip)
{
	char	*buf;
	int	xsize,ysize;
	int	x,y,n,t;
	char	b,b2;
        FFILE    *fp;

        fp=ffopen(filename,"rb");

	if(fp==0)
	{
		pcx_error("unable to open .pcx-file.",filename);
	}

	pcx_file_skipbyte(fp,128);

	buf=(char*)malloc(hdr->nplanes*hdr->bytesperline);

        if(buf==0)pcx_error("out of memory (loading .pcx-file.",filename);

	xsize=hdr->xmax-hdr->xmin+1;
	ysize=hdr->ymax-hdr->ymin+1;

	for(y=0;y<ysize;y++)
	{
		t=0;

		//read scanline*nplanes
//                for(n=0;n<hdr->nplanes;n++)
		{
			x=0;

                        while(x<hdr->bytesperline*hdr->nplanes)
			{
				//read data
				b=pcx_file_getbyte(fp);

				if((b&192)==192)
				{
					//encoded stream
					b2=pcx_file_getbyte(fp);
					b-=192;
					for(;b>0;b--)
					{
                                                if(t>=hdr->bytesperline*hdr->nplanes)break;
						buf[t]=b2;
						t++;
						x++;
					}
				}
				else
				{
					buf[t]=b;
					t++;
					x++;
				}
                        }
//                        printf("%i\n",x);
		}

		//dump scanline to picture
/*                for(n=0;n<hdr->nplanes;n++)
		{
			for(x=0;x<xsize;x++)
			{
                                pic[(y*xsize+x)*(hdr->nplanes+skip)+n]=buf[n*hdr->bytesperline+x];
			}
		}
  */
                for(x=0;x<xsize;x++)
                {
                        for(n=0;n<hdr->nplanes;n++)
                        {
                                pic[(y*xsize+x)*(hdr->nplanes+skip)+n]=buf[(hdr->nplanes-n-1)*hdr->bytesperline+x];
			}
                        for(n=hdr->nplanes;n<hdr->nplanes+skip;n++)
                        {
                                pic[(y*xsize+x)*(hdr->nplanes+skip)+n]=0;
			}
		}
        }

	free(buf);

        ffclose(fp);
}

char*   pcx_read_picture(char *filename, int skip)
{
	pcx_header	hdr;
	char		*pic;
        int             add2lines=skip&&PCX_ADD2LINES;      //<--variabel oprettet 11. marts 1999

        skip=skip&&65535;

	//loading af header
	pcx_read_header(&hdr,filename);

	if(hdr.manufacturer!=10)
	{
		pcx_error("not .pcx-format.",filename);
	}
	if(hdr.version!=5)
	{
		pcx_error("this reader supports only version 5.",filename);
	}
	if(hdr.encoding!=1)
	{
		pcx_error("this reader supports only encoding type 1.",filename);
	}
	if(hdr.bitsperpixel!=8)
	{
		pcx_error("this reader supports only 8 bits per pixel.",filename);
	}

        if(add2lines)hdr.ymax+=2;
        pic=(char*)malloc((hdr.xmax-hdr.xmin+1)*(hdr.ymax-hdr.ymin+1)*(hdr.nplanes+skip));
        if(pic==0)pcx_error("out of memory (loading .pcx-file.",filename);

	//loading af data
        if(add2lines)hdr.ymax-=2;
        pcx_read_data(&hdr,filename,pic,skip);

        if(add2lines)
        {
                int w,s;
                w=(hdr.xmax-hdr.xmin+1);
                s=w*(hdr.ymax-hdr.ymin+1)*(hdr.nplanes+skip);
                for(w=0;w<2*(hdr.xmax-hdr.xmin+1);w++)
                {
                        int i;

                        for(i=0;i<hdr.nplanes;i++)
                        {
                                pic[s+w*(hdr.nplanes+skip)+i]=pic[w*(hdr.nplanes+skip)+i];
                        }
                }
        }

	return pic;
}

char*	pcx_read_palette(char *filename)
{
        FFILE    *fp;
	int	p,t;
	char	*pal;

        fp=ffopen(filename,"rb");

	if(fp==0)
	{
		pcx_error("unable to open .pcx-file.",filename);
	}

        assert(NULL);
 //       fseek(fp,-769,SEEK_END);

	p=pcx_file_getbyte(fp);

	if(p!=12)
	{
		pcx_error("image contains no palette.",filename);
	}

	pal=(char*)malloc(768);

	for(t=0;t<768;t++)
	{
		p=pcx_file_getbyte(fp);
		pal[t]=p/4;
	}

        ffclose(fp);

        return pal;
}

#endif
