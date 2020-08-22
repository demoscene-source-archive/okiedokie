#include <stdio.h>
#include <stdlib.h>
#include "filer.h"
#include "file3ds.h"

int     ds_3dspos;

ds_chunk   ds_readchunk(FFILE* fil)
{
        ds_chunk c;
        c.id=ffgetword(fil);
        c.l=ffgetdword(fil);
        ds_3dspos+=6;
        c.endp=c.l+ds_3dspos-6;
	return c;
}

void    ds_endchunk(FFILE* fil,ds_chunk c)
{
        for(;ds_3dspos<c.endp;ds_3dspos++)ffgetc(fil);

        if(ffeof(fil))
	{
      //          setmode(3);
                printf("EOF -fejl (ds_endchunk)\n");
		exit(1);
	}
}

int     ds_inchunk(ds_chunk c)
{
        return  (c.endp>ds_3dspos);
}

int   ds_readword(FFILE* fil)
{
        int   i;
        i=ffgetword(fil);
        ds_3dspos+=2;
        return  i;
}

int   ds_readsword(FFILE* fil)
{
        int   i;
        i=ffgetword(fil);
        ds_3dspos+=2;
        i=(signed short)i;
        return  i;
}

int     ds_readdword(FFILE* fil)
{
	int	i;
        i=ffgetdword(fil);
        ds_3dspos+=4;
        return  i;
}

float   ds_readfloat(FFILE* fil)
{
	float	f;
        (*((int*)(&f)))=ds_readdword(fil);
	return	f;
}

float   ds_readfloats(FFILE* fil)
{
        float   f;

        (*((int*)(&f)))=ds_readdword(fil);
        return  f;
}

char    ds_readbyte(FFILE* fil)
{
        ds_3dspos++;
        return  ffgetbyte(fil);
}

void    ds_readname(FFILE* fil,char* s)
{
	int	t;
	t=0;
        s[t]=ds_readbyte(fil);
	while(s[t]!=0)
	{
		t++;
                s[t]=ds_readbyte(fil);
	}
}






          /*

float   ds_det(float a,float b,float c,float d)
{
	return a*d-b*c;
}

float   ds_len(float x,float y,float z)
{
	return sqrt(x*x+y*y+z*z);
}
void    ds_calcnorm(float x1,float y1,float z1,float x2,float y2,float z2,float x3,float y3,float z3,float *a,float *b,float *c)
{
	float l;
	x2-=x1;
	y2-=y1;
	z2-=z1;
	x3-=x1;
	y3-=y1;
	z3-=z1;
        (*a)=ds_det(y2,z2,y3,z3);
        (*b)=ds_det(z2,x2,z3,x3);
        (*c)=ds_det(x2,y2,x3,y3);

        l=ds_len((*a),(*b),(*c));

	(*a)*=128/l;
	(*b)*=128/l;
	(*c)*=128/l;
}
*/
int     ds_cmpnames(char *a,char* b)
{
	int f,c;
	f=1;
	c=0;
	while((a[c]!=0)&&(a[c]!='*')&&(b[c]!=0)&&(b[c]!='*'))
	{
		if ((a[c]!='#')&&(b[c]!='#')) f=f&&(a[c]==b[c]);
		c++;
	}
	if((a[c]==0)&&(b[c]!=0)&&(b[c]!='*'))f=0;
	if((b[c]==0)&&(a[c]!=0)&&(a[c]!='*'))f=0;

	return f;
}

