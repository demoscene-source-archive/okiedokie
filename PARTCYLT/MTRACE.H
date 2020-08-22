#ifndef _mTRACE_H_
#define _mTRACE_H_
#include "..\demolib\matrix.h"
#include <math.h>

typedef struct trace_object
{
    float   a,b,c,d,e,f,g,h,i,j;
} trace_object;

trace_object trace_translate(trace_object obj, fmatrix *mat);
int     trace_inside(trace_object obj, float x, float y, float z);
int     trace_inside2(trace_object *obj, float x, float y, float z);
fvertex trace_gradient(trace_object obj, float x, float y, float z);
float   trace_intersect(trace_object obj, float x, float y, float z);
float   trace_intersect3(trace_object *obj, float x, float y, float z);
float   trace_intersect2(trace_object *obj, float x, float y);
//void    trace_draw(char *dest);

#endif

