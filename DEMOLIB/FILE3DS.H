#ifndef FILE3DS
#define FILE3DS
#include <stdio.h>
#include "filer.h"

typedef struct ds_chunk
{
	short	id;
    int     l,endp;
} ds_chunk;

ds_chunk ds_readchunk(FFILE* fil);
void    ds_endchunk(FFILE* fil,ds_chunk c);
int     ds_inchunk(ds_chunk c);
int     ds_readword(FFILE* fil);
int     ds_readsword(FFILE* fil);
int     ds_readdword(FFILE* fil);
float   ds_readfloat(FFILE* fil);
float   ds_readfloats(FFILE* fil);
char    ds_readbyte(FFILE* fil);
void    ds_readname(FFILE* fil,char* s);
int     ds_cmpnames(char *a,char* b);

#endif
