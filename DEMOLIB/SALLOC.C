#include "structs.h"
#include "matrix.h"
#include "quat.h"
#include        "th.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

fvertex VAL_b_fvertex(float x, float y, float z)
{
        fvertex v;

        v.x=x;
        v.y=y;
        v.z=z;
        return v;
}
b_face VAL1_b_face(fvertex* a, fvertex* b, fvertex* c, int maptype, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, b_texture* texture)
{
        b_face v;

        v.sortnext=NULL;
        v.a=a;
        v.b=b;
        v.c=c;
        v.na=NULL;
        v.nb=NULL;
        v.nc=NULL;
        v.fA=NULL;
        v.fB=NULL;
        v.fC=NULL;
        v.maptype=maptype;
        v.visible=0;
        v.tx1=tx1;
        v.ty1=ty1;
        v.tx2=tx2;
        v.ty2=ty2;
        v.tx3=tx3;
        v.ty3=ty3;
        v.texture=texture;
        v.tiny=NULL;

        return v;
}
/*
b_face* INIT_b_face()
{
        fvertex v;

        v->x=x;
        v->y=y;
        v->z=z;
        return v;
}
*/
b_facelist A_b_facelist(int n)
{
        int     t;
        b_facelist f;

        assert(f=malloc(sizeof(b_face)*n));

        for(t=0;t<n;t++)
        f[t]=VAL1_b_face(NULL,NULL,NULL,0,0,0,0,0,0,0,NULL);

        return f;
}

b_verlist A_b_verlist(int n)
{
        b_verlist f;

        assert(f=malloc(sizeof(fvertex)*n));
        return f;
}

void    FREE_b_facelist(b_facelist s)
{
        free(s);
}

void    FREE_b_verlist(b_verlist s)
{
        free(s);
}

string  A_string(const char *s)
{
        char *t;
        if(s)
        {
                t=(void*)malloc(strlen(s)+1);
                assert(t);
                return strcpy(t,s);
        }
        return NULL;
}

void    FREE_string(char *s)
{
        free(s);
}

b_object*       A_b_object(char *name, b_facelist facelist, int numfaces, b_verlist verlist, int numvertices, b_verlist verdestlist)
{
        b_object*       s;

        s=malloc(sizeof(b_object));
        assert(s);

        s->next=s->prev=s;
        s->hnext=s->hprev=0;

        s->on=1;

        resetmatrixf(&s->mat);
        resetmatrixf(&s->nat);
        s->rotationtype=b_quatrot;
        s->quat=quat_makerotate(1,0,0,0);
        s->rotx=s->roty=s->rotz=0;
        s->scalex=s->scaley=s->scalez=1;

        s->pivotx=0;
        s->pivoty=0;
        s->pivotz=0;

        s->x=0;
        s->y=0;
        s->z=0;

        resetmatrixf(&s->tm);

        s->realtimenormals=0;

        s->boundon=0;
        s->boundx=0;
        s->boundy=0;
        s->boundz=0;
        s->boundradius=0;

        s->facelist=facelist;
        s->tinyfaces=NULL;
        s->verlist=verlist;
        s->verdestlist=verdestlist;

        s->norlist=NULL;
        s->nordestlist=NULL;
        s->numnormals=0;

        s->numvertices=numvertices;
        s->numfaces=numfaces;

        s->spec=NULL;
        s->spect4=s->spect3=s->spect2=s->spect1=0;

        s->name=A_string(name);
        s->keyframenr=-1;

        s->posspline.x=NULL;
        s->posspline.y=NULL;
        s->posspline.z=NULL;
        s->qspline=NULL;

        return s;
}

void    FREE_b_object(b_object *s)
{
        if(s)
        {
                FREE_string(s->name);
                free(s);
        }
        else
        {

        }
}

b_camera*       A_b_camera(b_scene *scene)
{
        b_camera* s;

        assert(s=malloc(sizeof(b_camera)));

        s->scene=scene;

        s->camz=1000;s->camy=s->camx=0;
        s->tgtz=s->tgty=s->tgtx=0;
        s->roll=0;

        s->scalez=s->scaley=s->scalex=1;

        s->clipleft=0;
        s->clipright=320;
        s->clipbottom=0;
        s->cliptop=200;

        s->viewportwidth=320;
        s->viewportheight=200;

        s->cuton=0;
        s->cutpz=s->cutpy=s->cutpx=0;
        s->cutvz=s->cutvy=s->cutvx=0;

        s->tgtspline.x=NULL;
        s->tgtspline.y=NULL;
        s->tgtspline.z=NULL;

        s->camspline.x=NULL;
        s->camspline.y=NULL;
        s->camspline.z=NULL;

        return s;
}

void    FREE_b_camera(b_camera *s)
{
        free(s);
}

b_scene* A_b_scene(char *texfile)
{
        b_scene* s;

        assert(s=malloc(sizeof(b_scene)));

        s->root=(A_b_object("Root",NULL,0,NULL,0,NULL));

        th_inittexturehandler(&s->th,texfile);

        return s;
}

void    FREE_b_scene(b_scene *s)
{
        free(s);
}


/*
spline_key      *A_spline_key(spline_key *prev,spline_key *next,spline_key *j,float t,float f,float fm,float a,float b)
{
        spline_key      *k;
        assert(k=malloc(sizeof(spline_key)));

        k->prev=prev;
        k->next=next;
        k->j=j;
        k->t=t;
        k->f=f;
        k->fm=fm;
        k->a=a;
        k->b=b;

        return k;
}

void    FREE_spline_key(spline_key *k)
{
        free(k);
}
*/
/*
qspline_key      *A_qspline_key(qspline_key *prev,qspline_key *next,qspline_key *j,float t, quat q, quat a)
{
        qspline_key      *k;
        assert(k=malloc(sizeof(qspline_key)));

        k->prev=prev;
        k->next=next;
        k->j=j;
        k->t=t;
        k->q=q;
        k->a=a;

        return k;
}

void    FREE_qspline_key(qspline_key *k)
{
        free(k);
}
*/
