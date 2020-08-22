#ifndef META
#define META


#include <stdio.h>
#include <math.h>

#include "..\demolib\matrix.h"
#include "..\demolib\structs.h"

typedef struct metaball
{
        float   x,y,z;
        float   dz,dzy;
        float   size0;
        float   size;
        float   ssinv;
        float   t1,t2,t3,t4;
        struct metaball *next1;
        struct metaball *next2;
        struct metaball *next3;
} metaball;


extern int     meta_vertexnum;
extern int     meta_nf;

void    meta(b_face *faces, fvertex *vertices, fvertex *destvertices, fvertex *normals, fvertex *destnormals, metaball *balls2, int startx, int starty, int startz, float v);
void    meta_cleargrid(void);


#endif
