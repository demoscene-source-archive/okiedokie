#ifndef quat_h_
#define quat_h_
#include "matrix.h"
#include <math.h>

typedef struct quat{
    float   a;
    float   x,y,z;
} quat;

#include "structs.h"

void    quat_print(quat q);
void    quat_printvertex(vertex q);
vertex  quat_vertex(float x, float y, float z);
quat    quat_vertextoquat(vertex v);
vertex  quat_quattovertex(quat q);
quat    quat_conjugate(quat q);
quat    quat_multiply(quat p, quat q);
quat    quat_trimultiply(quat p, quat q, quat r);
vertex  quat_transform(quat q, vertex v);
void    quat_quattomatrix(quat q, fmatrix* m);
quat    quat_makerotatex(float a);
quat    quat_makerotatey(float a);
quat    quat_makerotatez(float a);
quat    quat_makerotate(float x, float y, float z, float a);
void    quat_rotatex(quat *q, float a);
void    quat_rotatey(quat *q, float a);
void    quat_rotatez(quat *q, float a);
void    quat_rotate(quat *q, float x, float y, float z, float a);
void    quat_test();
float   quat_dot(quat p, quat q);
quat    quat_mulscalar(quat q, float s);
quat    quat_add(quat q, quat p);
quat    quat_slerp(quat p, quat q, float u);
quat    quat_exp(quat q);
quat    quat_ln(quat q);
quat    quat_squad(quat a, quat b, quat c, quat d, float u);
quat    quat_quadranglepoint(quat qm,quat q,quat qp);

#endif
