#include "mtrace.h"

#define trace_size 8

trace_object objects[10];

trace_object trace_translate(trace_object obj, fmatrix *mat)
{
        trace_object obj2;

        obj2.a=obj.a*mat->xu*mat->xu+obj.b*mat->yu*mat->yu+obj.c*mat->zu*mat->zu
               +obj.d*mat->xu*mat->yu+obj.e*mat->xu*mat->zu+obj.f*mat->yu*mat->zu;
        obj2.b=obj.a*mat->xv*mat->xv+obj.b*mat->yv*mat->yv+obj.c*mat->zv*mat->zv
               +obj.d*mat->xv*mat->yv+obj.e*mat->xv*mat->zv+obj.f*mat->yv*mat->zv;
        obj2.c=obj.a*mat->xw*mat->xw+obj.b*mat->yw*mat->yw+obj.c*mat->zw*mat->zw
               +obj.d*mat->xw*mat->yw+obj.e*mat->xw*mat->zw+obj.f*mat->yw*mat->zw;

        obj2.d=obj.a*2*mat->xu*mat->xv+obj.b*2*mat->yu*mat->yv+obj.c*2*mat->zu*mat->zv
               +obj.d*mat->xu*mat->yv+obj.e*mat->xu*mat->zv+obj.f*mat->yu*mat->zv
               +obj.d*mat->yu*mat->xv+obj.e*mat->zu*mat->xv+obj.f*mat->zu*mat->yv;
        obj2.e=obj.a*2*mat->xu*mat->xw+obj.b*2*mat->yu*mat->yw+obj.c*2*mat->zu*mat->zw
                +obj.d*mat->xu*mat->yw+obj.e*  mat->xu*mat->zw+obj.f*  mat->yu*mat->zw
                +obj.d*mat->yu*mat->xw+obj.e*  mat->zu*mat->xw+obj.f*  mat->zu*mat->yw;
        obj2.f=obj.a*2*mat->xw*mat->xv+obj.b*2*mat->yw*mat->yv+obj.c*2*mat->zw*mat->zv
                +obj.d*mat->xw*mat->yv+obj.e*  mat->xw*mat->zv+obj.f*  mat->yw*mat->zv
                +obj.d*mat->yw*mat->xv+obj.e*  mat->zw*mat->xv+obj.f*  mat->zw*mat->yv;

        obj2.g=obj.a*2*mat->xu*mat->xx+obj.b*2*mat->yu*mat->yy+obj.c*2*mat->zu*mat->zz
               +obj.d*mat->xu*mat->yy+obj.e*mat->xu*mat->zz+obj.f*mat->yu*mat->zz
               +obj.d*mat->yu*mat->xx+obj.e*mat->zu*mat->xx+obj.f*mat->zu*mat->yy
               +obj.g*mat->xu+obj.h*mat->yu+obj.i*mat->zu;
        obj2.h=obj.a*2*mat->xv*mat->xx+obj.b*2*mat->yv*mat->yy+obj.c*2*mat->zv*mat->zz
                +obj.d*mat->xv*mat->yy+obj.e*  mat->xv*mat->zz+obj.f*  mat->yv*mat->zz
                +obj.d*mat->yv*mat->xx+obj.e*  mat->zv*mat->xx+obj.f*  mat->zv*mat->yy
                +obj.g*mat->xv        +obj.h*  mat->yv        +obj.i*  mat->zv;
        obj2.i=obj.a*2*mat->xw*mat->xx+obj.b*2*mat->yw*mat->yy+obj.c*2*mat->zw*mat->zz
                +obj.d*mat->xw*mat->yy+obj.e*  mat->xw*mat->zz+obj.f*  mat->yw*mat->zz
                +obj.d*mat->yw*mat->xx+obj.e*  mat->zw*mat->xx+obj.f*  mat->zw*mat->yy
                +obj.g*mat->xw        +obj.h*  mat->yw        +obj.i*  mat->zw;

        obj2.j=obj.a*mat->xx*mat->xx+obj.b*mat->yy*mat->yy+obj.c*mat->zz*mat->zz
               +obj.d*mat->xx*mat->yy+obj.e*mat->xx*mat->zz+obj.f*mat->yy*mat->zz
               +obj.g*mat->xx+obj.h*mat->yy+obj.i*mat->zz+ obj.j; //f rettet til i!



        return obj2;
}

int     trace_inside(trace_object obj, float x, float y, float z)
{
        return ((obj.a*x*x+obj.b*y*y+obj.c*z*z
        +obj.d*x*y+obj.e*x*z+obj.f*y*z
        +obj.g*x+obj.h*y+obj.i*z+obj.j)>=0);
}

int     trace_inside2(trace_object *obj, float x, float y, float z)
{
        return (((obj->a*x+obj->d*y+obj->g+obj->e*z)*x
        +(obj->b*y+obj->f*z+obj->h)*y
        +(obj->i+obj->c*z)*z+obj->j)>=0);
}

fvertex trace_gradient(trace_object obj, float x, float y, float z)
{
        fvertex v;

        v.x=2*obj.a*x+obj.d*y+obj.e*z+obj.g;
        v.y=2*obj.b*y+obj.d*x+obj.f*z+obj.h;
        v.z=2*obj.c*z+obj.e*x+obj.f*y+obj.i;

        return v;
}

float   trace_intersect(trace_object obj, float x, float y, float z)
{
        float   alfa,beta,gamma;
        float   diskriminant;

        alfa=obj.a*x*x+obj.b*y*y+obj.c*z*z+obj.d*x*y+obj.e*x*z+obj.f*y*z;
        beta=obj.g*x+obj.h*y+obj.i*z;
        gamma=obj.j;

        diskriminant=beta*beta-4*alfa*gamma;

        if(diskriminant<0)return -1;

        return (-beta+sqrt(diskriminant))/(2*alfa);
}

float   trace_intersect3(trace_object *obj, float x, float y, float z)
{
        float   alfa,beta,gamma;
        float   diskriminant;

        alfa=obj->a*x*x+obj->b*y*y+obj->c*z*z+obj->d*x*y+obj->e*x*z+obj->f*y*z;
        beta=obj->g*x+obj->h*y+obj->i*z;
        gamma=obj->j;

        diskriminant=beta*beta-4*alfa*gamma;

        if(diskriminant<0)return -1;

        return (-beta+sqrt(diskriminant))/(2*alfa);
}

float   trace_intersect2(trace_object *obj, float x, float y)
{
        float   alfa,beta,gamma;
        float   diskriminant;

        alfa=(obj->a*x+obj->e)*x+(obj->b*y+obj->d*x+obj->f)*y+obj->c;
        beta=obj->g*x+obj->h*y+obj->i;
        gamma=obj->j;

        diskriminant=beta*beta-4*alfa*gamma;

        if(diskriminant<0)return -1;

        return (-beta+sqrt(diskriminant))/(2*alfa);
}

