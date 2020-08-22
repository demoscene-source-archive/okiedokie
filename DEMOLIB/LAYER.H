#ifndef __LAYER_H
#define __LAYER_H

#include "layerasm.h"
#include "pcx.h"
#include "convert.h"

typedef struct  mix{
        int     type;
        int     r;
}mix;


typedef struct layer{
        void    *source;
        int     x,y;
        int     sizex,sizey;
        mix     mix;
}layer;

typedef struct layer4{
        void    *source,*dest;
        int     sourcemodulo,destmodulo;
        int     sizex,sizey;
        mix     mix;
        struct layer4 *next;
}layer4;


extern void*   layer_screen;

void    layer_init(void);

layer   layer_make(void *source, int x, int y, int sx, int sy, int type, int r);
layer   layer_loadimage(char* imagename);
layer   layer_loadimageandmask(char* imagename, char* maskname);
void    layer_makemask(layer l, int v);
void    layer_add(layer l);
void    layer_reset();
void    layer_draw();

layer   layer_printf(layer l, char *s);
void    layer_setfont(void *font);
layer   layer_maketext(int x, int y, int sx);

void    layer_draw15(void *dest);
void    layer_draw24(void *dest);
void    layer_draw32(void *dest);
layer   layer_hblur(layer l, float n);
layer   layer_hblurfast(layer l, float n);

int     RGB(int r, int g, int b);

#endif
