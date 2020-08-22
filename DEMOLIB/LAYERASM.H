#ifndef __LAYERASM_h
#define __LAYERASM_h

#pragma aux layer_drawasm "_*" parm caller []
extern void layer_drawasm(void *l);

#pragma aux layer_initasm "_*" parm caller []
extern void layer_initasm(void);

#pragma aux layer_hblurasm "_*" parm caller []
extern void layer_hblurasm(void *source,void *dest,int sizey, int sizex, int m, int g, float mul);

#pragma aux layer_hblurasmfast "_*" parm caller []
extern void layer_hblurasmfast(void *source,void *dest,int x, int mul);

#endif
