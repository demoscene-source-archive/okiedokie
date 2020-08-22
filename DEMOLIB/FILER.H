#ifndef __filer_h
#define __filer_h

#include <stdio.h>

int     f_getbyte(FILE *a);
void    f_putbyte(int b,FILE *a);
int     f_getword(FILE *a);
int     f_getsword(FILE *a);
void    f_putword(int b, FILE *a);
int     f_getbigword(FILE *a);
int     f_getbigsword(FILE *a);
void    f_putbigword(int b, FILE *a);
int     f_getdword(FILE *a);
void    f_putdword(int b, FILE *a);
int     f_getbigdword(FILE *a);
void    f_putbigdword(int b, FILE *a);
void    f_skip(int j,FILE *f);

#endif

#ifndef filrutiner
#define filrutiner

FILE*   ff_open(char *a, char *b);
void    ff_close(FILE *a);
int     ff_getbyte(FILE *a);
void    ff_putbyte(int b,FILE *a);
int     ff_getword(FILE *a);
int     ff_getsword(FILE *a);
void    ff_putword(int b, FILE *a);
int     ff_getbigword(FILE *a);
int     ff_getbigsword(FILE *a);
void    ff_putbigword(int b, FILE *a);
int     ff_getdword(FILE *a);
void    ff_putdword(int b, FILE *a);
int     ff_getbigdword(FILE *a);
void    ff_putbigdword(int b, FILE *a);
void    ff_skip(int j,FILE *f);

#endif

#include "file.h"

