#ifndef salloc
#define salloc

#include "structs.h"
#include "matrix.h"

fvertex VAL_b_fvertex(float x, float y, float z);
b_face VAL1_b_face(fvertex* a, fvertex* b, fvertex* c, int maptype, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, b_texture* texture);
b_verlist A_b_verlist(int n);
b_facelist A_b_facelist(int n);
void    FREE_b_facelist(b_facelist s);
void    FREE_b_verlist(b_verlist s);
string  A_string(const char *s);
void    FREE_string(char *s);
b_object *A_b_object(char *name, b_facelist facelist, int numfaces, b_verlist verlist, int numvertices, b_verlist verdestlist);
void    FREE_b_object(b_object *s);
b_camera*       A_b_camera(b_scene *scene);
void    FREE_b_camera(b_camera *s);
b_scene* A_b_scene(char *texfile);
void    FREE_b_scene(b_scene *s);
//spline_key      *A_spline_key(spline_key *prev,spline_key *next,spline_key *j,float t,float f,float fm,float a,float b);
//void    FREE_spline_key(spline_key *k);
//qspline_key     *A_qspline_key(qspline_key *prev,qspline_key *next,qspline_key *j,float t, quat q, quat a);
//void    FREE_qspline_key(qspline_key *k);



#endif
