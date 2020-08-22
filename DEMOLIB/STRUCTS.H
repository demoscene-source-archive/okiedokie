#ifndef b_structs
#define b_structs

#include "matrix.h"
#include "quat.h"

#define string char*
#define B_MAX_TEXTURES 256

#ifndef pi
#define pi      3.1415926535897932
#endif

typedef struct
{
	void		*location;
    char        name[20];
	char		path[128];
	int		width,height;
	int		texturetype;
	int		bits;
} b_texture;




typedef struct th_texturehandler
{
	int		numslots;	//number of tex-slots
	b_texture	*slots; 	//pointer to slots
} th_texturehandler;




typedef struct b_tinyface
{
    struct b_face *sortnext;
    unsigned short a,b,c;
    unsigned short visible;
} b_tinyface;


typedef struct b_face
{
	struct b_face	*sortnext;
	fvertex 	*a,*b,*c;
	fvertex 	*na,*nb,*nc;
	struct b_face	*fA,*fB,*fC;
    int     maptype;                //benyttes ikke!
	int		visible;
	float		tx1,ty1,tx2,ty2,tx3,ty3;
	b_texture	*texture;
    b_tinyface  *tiny;
} b_face;

typedef struct b_face *b_facelist;
typedef struct fvertex *b_verlist;

typedef struct keyframe_multispline
{
    struct  spline_key *x,*y,*z;
}keyframe_multispline;

typedef enum{b_matrixrot,b_eulerrot,b_quatrot}b_rotationtype;
typedef struct b_object
{
    struct b_object     *next,*prev;
    int                 hnext,hprev;
    int                 on;
    fmatrix             mat;
    fmatrix             nat;
//	  int		  euler;
    quat                quat;
    float               rotx,roty,rotz;
    float               scalex,scaley,scalez;
    float               pivotx,pivoty,pivotz;
    float               x,y,z;
    fmatrix             tm;
    int                 realtimenormals;
    int                 boundon;
    float               boundx,boundy,boundz;
    float               boundradius;
    b_facelist          facelist;
    struct b_tinyface   *tinyfaces;
    b_verlist           verlist,verdestlist;
    b_verlist           norlist,nordestlist;
    int                 numvertices,numfaces,numnormals;
    void                *spec;
    float               spect1,spect2,spect3,spect4;
    string              name;
    int                 keyframenr;
    struct keyframe_multispline     posspline;
    struct qspline_key  *qspline;
    b_rotationtype      rotationtype;
} b_object;

typedef struct
{
    th_texturehandler th;
	b_object	*root;
} b_scene;

typedef struct
{
	b_scene 	*scene;
	float		camx,camy,camz;
	float		tgtx,tgty,tgtz;
	float		roll;
	float		scalex,scaley,scalez;
	int		clipleft,clipright,clipbottom,cliptop;
	int		viewportwidth,viewportheight;
	int		cuton;
	float		cutpx,cutpy,cutpz;
	float		cutvx,cutvy,cutvz;
	struct keyframe_multispline camspline;
	struct keyframe_multispline tgtspline;
} b_camera;


extern	b_texture      b_textures[B_MAX_TEXTURES];


typedef struct spline_key *spline_ke;
typedef struct spline_key{
	spline_ke prev,next,j;	 //j skal kun bruges i 'mainkey' til at vise den spline der blev beregnet v‘rdi fra sidst
	float	t;
	float	f,fm,a,b;
} spline_key;

typedef struct qspline_key *qspline_ke;
typedef struct qspline_key{
	qspline_ke prev,next,j;
	float	t;
	quat	q,a;
} qspline_key;


#endif
