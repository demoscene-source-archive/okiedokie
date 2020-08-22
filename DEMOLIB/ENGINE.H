#ifndef __engine
#define __engine


#include "berror.h"
#include "salloc.h"
#include "structs.h"
#include "matrix.h"
#include "bilerp.h"
#include    "th.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

void b_generatetinyfaceinfo(b_object *obj);
void b_initsimplenormals(b_object *obj);
void b_calcnormals(b_object *obj);
void    b_insertobject(b_object *son, b_object *father, int relativelevel);
void    b_insertobjectinscene(b_scene *scene, b_object *object);
void    b_printhierarki(b_object *object);
b_object* b_findobject(b_object* object, string s);
void    b_destroysingleobject(b_object *object);
b_object*    b_removeobjecttree(b_object *object);
void    b_calculate_compobjectmatrix(b_object *object, fmatrix *mat);
void    b_calculate_objectmatrix(b_object *object);
void    b_transformobjects(b_object *object);
void    b_testvisibility(b_object *object);
void    b_sort(b_scene *scene);
void    b_draw(void *screen0, void *screen1);
void    b_render(b_camera *cam, void *screen0, void *screen1);
void    b_generateneighbourinfo(b_facelist fl, int n);
b_object *b_tetrahedron(char* s);
void    b_inittimer();
void    b_init();
int     b_getrelativelevel(b_object* obj1, b_object* obj2);
b_object *b_findobjectnr(b_object* object, int nr);
void    b_changeobjectlevelrelative(b_object *obj, int level);
void    b_exchangeobjects(b_object *a, b_object *b);

b_verlist b_cloneverlist(b_verlist l, int n);

#endif
