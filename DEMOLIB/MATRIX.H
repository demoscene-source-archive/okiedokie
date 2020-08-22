#ifndef matrix
#define matrix

typedef struct fmatrix{
        float   xu,xv,xw,xx;
        float   yu,yv,yw,yy;
        float   zu,zv,zw,zz;
} fmatrix;

typedef struct fvertex{
        float   x,y,z;
} fvertex;

typedef struct vertex{
        float   x,y,z;
} vertex;

void    resetmatrixf(fmatrix *m);
void    scalematrixf(fmatrix *m, float x, float y, float z);
void    rotatematrixzf(fmatrix *m, float v);
void    rotatematrixyf(fmatrix *m, float v);
void    rotatematrixxf(fmatrix *m, float v);
void    movematrixf(fmatrix *m, float x, float y, float z);
void    fmovematrixf(fmatrix *m, float x, float y, float z);
void    frotatematrixzf(fmatrix *m, float v);
void    frotatematrixyf(fmatrix *m, float v);
void    frotatematrixxf(fmatrix *m, float v);
void    fscalematrixf(fmatrix *m, float x, float y, float z);
void    fmulmatrixf(fmatrix *m, fmatrix *n);
void    mulmatrixf(fmatrix *m, fmatrix *n);
void    rotatematrixf(fmatrix *m, float x, float y, float z, float v);
void    printmatrixf(fmatrix *m);
void    transformf(fmatrix *m, fvertex *s, fvertex *d, int n);

#endif
