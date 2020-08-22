#include "filer.h"
#include "file.c"

int     f_getbyte(FILE *a)
{
        return ((unsigned char)fgetc(a));
}

void    f_putbyte(int b,FILE *a)
{
        fputc(b,a);
}

int     f_getword(FILE *a)
{
        char    b[4];
        b[0]=f_getbyte(a);
        b[1]=f_getbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     f_getsword(FILE *a)
{
        char    b[4];
        b[0]=f_getbyte(a);
        b[1]=f_getbyte(a);
        return ((signed short*)b)[0];
}

void    f_putword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        f_putbyte(c[0],a);
        f_putbyte(c[1],a);
}

int     f_getbigword(FILE *a)
{
        char    b[4];
        b[1]=f_getbyte(a);
        b[0]=f_getbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     f_getbigsword(FILE *a)
{
        char    b[4];
        b[1]=f_getbyte(a);
        b[0]=f_getbyte(a);
        return ((signed short*)b)[0];
}

void    f_putbigword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        f_putbyte(c[1],a);
        f_putbyte(c[0],a);
}

int     f_getdword(FILE *a)
{
        char    b[4];
        b[0]=f_getbyte(a);
        b[1]=f_getbyte(a);
        b[2]=f_getbyte(a);
        b[3]=f_getbyte(a);
        return ((int*)b)[0];
}

void    f_putdword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        f_putbyte(c[0],a);
        f_putbyte(c[1],a);
        f_putbyte(c[2],a);
        f_putbyte(c[3],a);
}

int     f_getbigdword(FILE *a)
{
        char    b[4];
        b[3]=f_getbyte(a);
        b[2]=f_getbyte(a);
        b[1]=f_getbyte(a);
        b[0]=f_getbyte(a);
        return ((int*)b)[0];
}

void    f_putbigdword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        f_putbyte(c[3],a);
        f_putbyte(c[2],a);
        f_putbyte(c[1],a);
        f_putbyte(c[0],a);
}

void    f_skip(int j,FILE *f)
{
	for(;j>0;j--){fgetc(f);};
}


//Standard fil-rutiner
//Skrevet af Anders den 17/1'97

//#include <stdio.h>
//#include "filerout.h"

FILE*   ff_open(char *a, char *b)
{
        return fopen(a,b);
}

void    ff_close(FILE *a)
{
        fclose(a);
}

int     ff_getbyte(FILE *a)
{
        return ((unsigned char)fgetc(a));
}

void    ff_putbyte(int b,FILE *a)
{
        fputc(b,a);
}

int     ff_getword(FILE *a)
{
        char    b[4];
        b[0]=ff_getbyte(a);
        b[1]=ff_getbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     ff_getsword(FILE *a)
{
        char    b[4];
        b[0]=ff_getbyte(a);
        b[1]=ff_getbyte(a);
        return ((signed short*)b)[0];
}

void    ff_putword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        ff_putbyte(c[0],a);
        ff_putbyte(c[1],a);
}

int     ff_getbigword(FILE *a)
{
        char    b[4];
        b[1]=ff_getbyte(a);
        b[0]=ff_getbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     ff_getbigsword(FILE *a)
{
        char    b[4];
        b[1]=ff_getbyte(a);
        b[0]=ff_getbyte(a);
        return ((signed short*)b)[0];
}

void    ff_putbigword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        ff_putbyte(c[1],a);
        ff_putbyte(c[0],a);
}

int     ff_getdword(FILE *a)
{
        char    b[4];
        b[0]=ff_getbyte(a);
        b[1]=ff_getbyte(a);
        b[2]=ff_getbyte(a);
        b[3]=ff_getbyte(a);
        return ((int*)b)[0];
}

void    ff_putdword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        ff_putbyte(c[0],a);
        ff_putbyte(c[1],a);
        ff_putbyte(c[2],a);
        ff_putbyte(c[3],a);
}

int     ff_getbigdword(FILE *a)
{
        char    b[4];
        b[3]=ff_getbyte(a);
        b[2]=ff_getbyte(a);
        b[1]=ff_getbyte(a);
        b[0]=ff_getbyte(a);
        return ((int*)b)[0];
}

void    ff_putbigdword(int b, FILE *a)
{
        char    c[4];
        ((int*)c)[0]=b;
        ff_putbyte(c[3],a);
        ff_putbyte(c[2],a);
        ff_putbyte(c[1],a);
        ff_putbyte(c[0],a);
}

void    ff_skip(int j,FILE *f)
{
	for(;j>0;j--){fgetc(f);};
}

