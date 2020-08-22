/* vector engine by craft / fudge */

#include "berror.h"
#include "salloc.h"
#include "structs.h"
#include "matrix.h"
#include "bilerp.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "clockcyc.h"

cc_timer        enginetimer,transformationtimer,drawtimer,drawtimer2,sorttimer;

//#include <stdlib.h>

#define SORTSIZE 4096
//#define SORTSIZE 1024
#define SORTSCALE 0.2

void *engine_phongmap;


b_face  *sortt[SORTSIZE];

void b_generatetinyfaceinfo(b_object *obj)
{
        b_object *obj1=obj;

        if(!obj)return;

        do
        {
                int     t,numfaces;
                fvertex *verdestlist;
                b_facelist  facelist;
                b_tinyface      *tinyfaces;

                if(obj->tinyfaces)free(obj->tinyfaces);

                obj->tinyfaces=(b_tinyface*)malloc(sizeof(b_tinyface)*obj->numfaces);


                verdestlist=obj->verdestlist;
                numfaces=obj->numfaces;
                facelist=obj->facelist;
                tinyfaces=obj->tinyfaces;

                for(t=0;t<numfaces;t++)
                {
                        tinyfaces[t].a=facelist[t].a-verdestlist;
                        tinyfaces[t].b=facelist[t].b-verdestlist;
                        tinyfaces[t].c=facelist[t].c-verdestlist;

                        tinyfaces[t].sortnext=NULL;

                        facelist[t].tiny=&tinyfaces[t];
                }
                obj=obj->next;
        }
        while(obj1!=obj);
}


void b_initsimplenormals(b_object *obj)
{
        int     t;
        b_facelist faces;
        b_verlist  normals;
        b_verlist  vertices;

        obj->numnormals=obj->numvertices;
        obj->norlist=A_b_verlist(obj->numnormals);
        obj->nordestlist=A_b_verlist(obj->numnormals);


        faces=obj->facelist;
        normals=obj->nordestlist;
        vertices=obj->verdestlist;

        for(t=0;t<obj->numfaces;t++)
        {
                faces[t].na=normals+(faces[t].a-vertices);
                faces[t].nb=normals+(faces[t].b-vertices);
                faces[t].nc=normals+(faces[t].c-vertices);
        }

        b_calcnormals(obj);
}

void b_calcnormals(b_object *obj)
{
        int         t;
        float       x,y,z;
        float       ux,uy,uz;
        float       vx,vy,vz;
        fvertex     *a,*b,*c;
        b_verlist   normals;
        b_verlist   destnormals;
        b_verlist   vertices;
        b_verlist   destvertices;
        b_facelist  faces;
        int         nnor,nfac;

        normals=obj->norlist;
        destnormals=obj->nordestlist;
        vertices=obj->verlist;
        destvertices=obj->verdestlist;
        if(!normals)return;
        nnor=obj->numnormals;

        for(t=0;t<nnor;t++)
        {
                destnormals[t].x=0;
                destnormals[t].y=0;
                destnormals[t].z=0;
        }

        faces=obj->facelist;
        nfac=obj->numfaces;

        for(t=0;t<nfac;t++)
        {
                a=faces[t].a-destvertices+vertices;
                b=faces[t].b-destvertices+vertices;
                c=faces[t].c-destvertices+vertices;
                vx=a->x-b->x;
                vy=a->y-b->y;
                vz=a->z-b->z;
                ux=c->x-b->x;
                uy=c->y-b->y;
                uz=c->z-b->z;

                a=faces[t].na;
                b=faces[t].nb;
                c=faces[t].nc;
                x=vy*uz-uy*vz;
                y=vz*ux-uz*vx;
                z=vx*uy-ux*vy;
                a->x+=x;
                a->y+=y;
                a->z+=z;
                b->x+=x;
                b->y+=y;
                b->z+=z;
                c->x+=x;
                c->y+=y;
                c->z+=z;
        }

        for(t=0;t<nnor;t++)
        {
                float   l;

                x=destnormals[t].x;
                y=destnormals[t].y;
                z=destnormals[t].z;
                l=1.0/sqrt(x*x+y*y+z*z);
                normals[t].x=x*l;
                normals[t].y=y*l;
                normals[t].z=z*l;
        }

}

b_verlist b_cloneverlist(b_verlist l, int n)
{
        b_verlist       l2;
        int             t;

        l2=A_b_verlist(n);

        for(t=0;t<n;t++)l2[t]=l[t];
        return l2;
}
/*
b_object *b_clonesingleobject(b_object *o1)
{
        b_object        *o2;

        o2=A_b_object(o1->name,
        A_b_facelist(o1->numfaces),
        o1->numfaces,b_cloneverlist(o1->verlist,o1->numvertices),
        o1->numvertices,b_cloneverlist(o1->verdestlist,o1->numvertices));

        {
                verlist a,b;
                facelist c;
                string d;

                a=o2->verlist;
                b=o2->verdestlist;
                c=o2->facelist;
                d=o2->name;

                *o2=*o1;

                o2->verlist=a;
                o2->verdestlist=b;
                o2->facelist=c;
                o2->name=d;
        }
}
*/

void    b_insertobject(b_object *son, b_object *father, int relativelevel)
{
        b_object        *a,*b,*c,*d;
        int             temp;

        a=son;
        b=son->prev;
        c=father;
        d=father->next;

        c->next=a;
        a->prev=c;

        b->next=d;
        d->prev=b;

//        temp=b->hnext+d->hprev-relativelevel;
        temp=b->hnext+d->hprev-relativelevel;

        c->hnext=relativelevel;
        a->hprev=relativelevel;

        d->hprev=b->hnext=temp;
}

void    b_insertobjectinscene(b_scene *scene, b_object *object)
{
        b_insertobject(object,scene->root,0);
}

void    b_printhierarki(b_object *object)
{
        b_object        *start;
        int             level=0,t;

        start=object;
        printf("%s\n",object->name);
        level+=object->hnext;
        object=object->next;

        while(object!=start)
        {
                for(t=0;t<level;t++)printf(" ");
                printf("%s\n",object->name);
                level+=object->hnext;
                if(object->hnext!=object->next->hprev)printf("level error!\n");
                object=object->next;
        }
        if(level)printf("level error2\n");
}

b_object* b_findobject(b_object* object, string s)
{
        b_object        *start=object;

        if(!strcmp(object->name,s)) return object;
        object=object->next;

        while(object!=start)
        {
                if(!strcmp(object->name,s)) return object;
                object=object->next;
        }
        // b_error("Object not found:%s\n",s);
        printf("%s\n",s);
        b_error("Object not found");
        return object;
}

void    b_destroysingleobject(b_object *object)
{
        FREE_b_verlist(object->verlist);
        object->verlist=NULL;
        FREE_b_verlist(object->verdestlist);
        object->verdestlist=NULL;
        FREE_b_facelist(object->facelist);
        object->facelist=NULL;

        FREE_b_object(object);
}

b_object*    b_removeobjecttree(b_object *object)
{
        b_object        *obj2,*a,*b,*c,*d;
        int             level,temp1,temp2;

        obj2=object->next;
        level=object->hnext;
        while(level>0)
        {
                level+=obj2->hnext;
                obj2=obj2->next;
        }

        //obj2 er det f›rste object der skal bevares


        a=object;
        b=obj2->prev;
        c=object->prev;
        d=obj2;

        temp1=-level+b->hnext;
        temp2=level+c->hnext;

        a->prev=b;
        b->next=a;
        c->next=d;
        d->prev=c;

        a->hprev=temp1;
        b->hnext=temp1;
        c->hnext=temp2;
        d->hprev=temp2;

        return object;
}

void    b_calculate_compobjectmatrix(b_object *object, fmatrix *mat, fmatrix *nat)
{
        int     level=1;
        b_object *obj2;
        fmatrix *m[20];
        fmatrix *n[20];

        m[0]=mat;
        n[0]=nat;

        obj2=object;

        do
        {
                obj2->nat=obj2->mat;
                mulmatrixf(&obj2->mat,m[level-1]);
                mulmatrixf(&obj2->nat,n[level-1]);
                m[level]=&obj2->mat;
                n[level]=&obj2->nat;

                level+=obj2->hnext;
                obj2=obj2->next;
        }while(obj2!=object);
}

void    b_calculate_objectmatrix(b_object *object)
{
        b_object *obj2;

        obj2=object;

        do
        {
                switch(obj2->rotationtype)
                {
                        case b_eulerrot:
                                resetmatrixf(&obj2->mat);
                                movematrixf(&obj2->mat,obj2->x,obj2->y,obj2->z);
                                scalematrixf(&obj2->mat,obj2->scalex,obj2->scaley,obj2->scalez);
                                rotatematrixxf(&obj2->mat,obj2->rotx);
                                rotatematrixyf(&obj2->mat,obj2->roty);
                                rotatematrixzf(&obj2->mat,obj2->rotz);
                                break;

                        case b_quatrot:
                                quat_quattomatrix(obj2->quat,&obj2->mat);
                                movematrixf(&obj2->mat,obj2->pivotx,obj2->pivoty,obj2->pivotz);
                                movematrixf(&obj2->mat,obj2->x,obj2->y,obj2->z);
                                fmovematrixf(&obj2->mat,-obj2->pivotx,-obj2->pivoty,-obj2->pivotz);
                                scalematrixf(&obj2->mat,obj2->scalex,obj2->scaley,obj2->scalez);
                                break;

                        case b_matrixrot:
                                break;
                }
                obj2=obj2->next;
        }while(obj2!=object);
}

void    b_transformobjects(b_object *object)
{       //b›r ikke kaldes da dette arbejde er flyttet til b_sort
        b_object *obj2;

        obj2=object;
        do
        {
                transformf(&obj2->mat,obj2->verlist,obj2->verdestlist,obj2->numvertices);
                obj2=obj2->next;
        }while(obj2!=object);
}

void    b_testvisibility(b_object *object)
{       //b›r ikke kaldes da dette arbejde er flyttet til b_sort
        b_object *obj2;

        obj2=object;

        do
        {
                b_facelist        f;
                int             i,t;

                f=obj2->facelist;
                i=obj2->numfaces;

                for(t=0;t<i;t++)
                {
                        f[t].visible=
                        (f[t].a->x*(f[t].b->y*f[t].c->z-f[t].b->z*f[t].c->y)
                        -f[t].a->y*(f[t].b->x*f[t].c->z-f[t].b->z*f[t].c->x)
                        <f[t].a->z*(f[t].b->y*f[t].c->x-f[t].b->x*f[t].c->y));

                }
                obj2=obj2->next;
        }while(obj2!=object);
}


void    b_sort(b_scene *scene)
{
        b_object *obj2,*object;


        object=scene->root;

        obj2=object;

        do
        {
                b_facelist        f;
                int             i,t;

                if(obj2->on)
                {
                        f=obj2->facelist;
                        i=obj2->numfaces;
                        transformf(&obj2->mat,obj2->verlist,obj2->verdestlist,obj2->numvertices);
                        if(obj2->numnormals)
                        {
                                fmatrix mat;

                                if(obj2->realtimenormals)b_calcnormals(obj2);

                                mat=obj2->nat;
                                mat.xx=0;
                                mat.yy=0;
                                mat.zz=0;

                                transformf(&mat,obj2->norlist,obj2->nordestlist,obj2->numnormals);
                        }

        //        cc_timeron(&transformationtimer);
                        if(obj2->tinyfaces)
                        {
                                b_tinyface *f2;
                                fvertex *verlist;
                                f2=obj2->tinyfaces;
                                verlist=obj2->verdestlist;
                                for(t=0;t<i;t++)
                                {
                                        fvertex *a,*b,*c;

                                        a=verlist+f2[t].a;
                                        b=verlist+f2[t].b;
                                        c=verlist+f2[t].c;
                                        a=(fvertex*)((char*)verlist+(sizeof(fvertex)&8)*f2[t].a+(sizeof(fvertex)&4)*f2[t].a+(sizeof(fvertex)&(-13))*f2[t].a);
                                        b=(fvertex*)((char*)verlist+(sizeof(fvertex)&8)*f2[t].b+(sizeof(fvertex)&4)*f2[t].b+(sizeof(fvertex)&(-13))*f2[t].b);
                                        c=(fvertex*)((char*)verlist+(sizeof(fvertex)&8)*f2[t].c+(sizeof(fvertex)&4)*f2[t].c+(sizeof(fvertex)&(-13))*f2[t].c);
                                    /*    b=(char*)verlist+8*f2[t].b+4*f2[t].b;
                                        c=(char*)verlist+8*f2[t].c+4*f2[t].c;
                                     */   f2[t].visible=
                                        (a->x*(b->y*c->z-b->z*c->y)
                                        -a->y*(b->x*c->z-b->z*c->x)
                                        <a->z*(b->y*c->x-b->x*c->y));
                                        if(f2[t].visible)
                                        {
                                                int             z;

                                                z=(a->z+b->z+c->z)*SORTSCALE;
                                                if(z<0)z=0;
                                                if((z>=0)&&(z<SORTSIZE))
                                                {
                                                        f2[t].sortnext=sortt[z];
                                                        sortt[z]=&f[t];
                                                }
                                        }
                                }
                        }
        //        cc_timeroff(&transformationtimer);
                        else
                        for(t=0;t<i;t++)
                        {
                                f[t].visible=
                                (f[t].a->x*(f[t].b->y*f[t].c->z-f[t].b->z*f[t].c->y)
                                -f[t].a->y*(f[t].b->x*f[t].c->z-f[t].b->z*f[t].c->x)
                                <f[t].a->z*(f[t].b->y*f[t].c->x-f[t].b->x*f[t].c->y));
                                if(f[t].visible)
                                {
                                        int             z;

                                        z=(f[t].a->z+f[t].b->z+f[t].c->z)*SORTSCALE;
                                        if(z<0)z=0;
                                        if((z>=0)&&(z<SORTSIZE))
                                        {
                                                f[t].sortnext=sortt[z];
                                                sortt[z]=&f[t];
                                        }
                                }
                        }
                }

                obj2=obj2->next;
        }while(obj2!=object);

}

//void    b_destroyobjecttree
//void    b_calculatenormals();

void    b_draw(void *screen0, void *screen1)
{
        int     t;
        b_face  *f;

        for(t=SORTSIZE-1;t>=0;t--)
        {
                f=sortt[t];
                sortt[t]=NULL;

                while(f!=NULL)
                {
                        submap_pkt      p[3];

                        //tegn flade
                        p[0].x=f[0].a->x;
                        p[0].y=f[0].a->y;
                        p[0].z=f[0].a->z;

                        p[1].x=f[0].b->x;
                        p[1].y=f[0].b->y;
                        p[1].z=f[0].b->z;
                        p[2].x=f[0].c->x;
                        p[2].y=f[0].c->y;
                        p[2].z=f[0].c->z;

                        switch(f->texture->texturetype)
                        {
                                case 4:
                                        p[0].tx=65536*(128+127*f->na->x);
                                        p[0].ty=65536*(128+127*f->na->y);
                                        p[0].g=0;
                                        p[1].tx=65536*(128+127*f->nb->x);
                                        p[1].ty=65536*(128+127*f->nb->y);
                                        p[1].g=0;
                                        p[2].tx=65536*(128+127*f->nc->x);
                                        p[2].ty=65536*(128+127*f->nc->y);
                                        p[2].g=0;
                                        break;
                                default:
                                        p[0].tx=f->tx1*65536*256;
                                        p[0].ty=f->ty1*65536*256;
                                        p[0].g=1.5*128*256*256-f[0].a->z*50000;
                                        p[1].tx=f->tx2*65536*256;
                                        p[1].ty=f->ty2*65536*256;
                                        p[1].g=1.5*128*256*256-f[0].b->z*50000;
                                        p[2].tx=f->tx3*65536*256;
                                        p[2].ty=f->ty3*65536*256;
                                        p[2].g=0*256+1.5*128*256*256-f[0].c->z*50000;
                                        if(f[0].na)
                                        {
                                                p[0].g=63*256*256*(-f[0].na->z-1)*(-f[0].na->z-1);
                                                p[1].g=63*256*256*(-f[0].nb->z-1)*(-f[0].nb->z-1);
                                                p[2].g=63*256*256*(-f[0].nc->z-1)*(-f[0].nc->z-1);
                                        }
                                        break;
                        }



        cc_timeron(&drawtimer2);
                        submap3gklip(&p[0],&p[1],&p[2],f->texture->texturetype,f->texture->location,screen1);
        cc_timeroff(&drawtimer2);

                        if(f->texture->texturetype==8)
                        {
                                //phong skal mappes oven i

                                p[0].x=f[0].a->x;
                                p[0].y=f[0].a->y;
                                p[0].z=f[0].a->z;

                                p[1].x=f[0].b->x;
                                p[1].y=f[0].b->y;
                                p[1].z=f[0].b->z;
                                p[2].x=f[0].c->x;
                                p[2].y=f[0].c->y;
                                p[2].z=f[0].c->z;

                                p[0].tx=65536*(128+127*f->na->x);
                                p[0].ty=65536*(128+127*f->na->y);
                                p[0].g=0;
                                p[1].tx=65536*(128+127*f->nb->x);
                                p[1].ty=65536*(128+127*f->nb->y);
                                p[1].g=0;
                                p[2].tx=65536*(128+127*f->nc->x);
                                p[2].ty=65536*(128+127*f->nc->y);
                                p[2].g=0;
                                submap3gklip(&p[0],&p[1],&p[2],7,engine_phongmap,screen1);

                        }

                        if(f->tiny)
                        f=f->tiny->sortnext;
                        else
                        f=f->sortnext;
                }
        }

}

void    b_render(b_camera *cam, void *screen0, void *screen1)
{
        b_scene *scene=cam->scene;
        fmatrix mat;
        fmatrix nat;

        cc_timeron(&enginetimer);
//beregn cammatrix( =rootmatrix)
        {
                float   dx,dy,dz;

                static float rot;

                dx=cam->tgtx-cam->camx;
                dy=cam->tgty-cam->camy;
                dz=cam->tgtz-cam->camz;
                resetmatrixf(&mat);
                movematrixf(&mat,-cam->camx,-cam->camy,-cam->camz);
                rotatematrixyf(&mat,-atan2(dx,dz));
                rotatematrixxf(&mat,atan2(dy,sqrt(dx*dx+dz*dz)));

                nat=mat;

//                rot+=0.05;
                rotatematrixxf(&nat,rot);
                rotatematrixyf(&nat,rot*1.5);

                scalematrixf(&mat,1.2,1,1);
                scalematrixf(&mat,1.4,1.4,1.4);
                scalematrixf(&mat,256.0/160.0,256.0/100.0,1);
        }

        b_calculate_objectmatrix(scene->root);
        b_calculate_compobjectmatrix(scene->root,&mat,&nat);


        cc_timeron(&sorttimer);
        b_sort(scene);
        cc_timeroff(&sorttimer);

        cc_timeron(&drawtimer);
        b_draw(screen0,screen1);
        cc_timeroff(&drawtimer);
        cc_timeroff(&enginetimer);
}

void    b_generateneighbourinfo(b_facelist fl, int n)
{
        int     i,j;


        for(i=0;i<n;i++)fl[i].fA=fl[i].fB=fl[i].fC=NULL;

        for(i=0;i<n;i++)
        for(j=i+1;j<n;j++)
        {
                if((fl[i].a==fl[j].b)&&(fl[i].b==fl[j].a)){fl[j].fC=fl+i;fl[i].fC=fl+j;}
                if((fl[i].a==fl[j].a)&&(fl[i].b==fl[j].c)){fl[j].fB=fl+i;fl[i].fC=fl+j;}
                if((fl[i].a==fl[j].c)&&(fl[i].b==fl[j].b)){fl[j].fA=fl+i;fl[i].fC=fl+j;}

                if((fl[i].b==fl[j].b)&&(fl[i].c==fl[j].a)){fl[j].fC=fl+i;fl[i].fA=fl+j;}
                if((fl[i].b==fl[j].a)&&(fl[i].c==fl[j].c)){fl[j].fB=fl+i;fl[i].fA=fl+j;}
                if((fl[i].b==fl[j].c)&&(fl[i].c==fl[j].b)){fl[j].fA=fl+i;fl[i].fA=fl+j;}

                if((fl[i].c==fl[j].b)&&(fl[i].a==fl[j].a)){fl[j].fC=fl+i;fl[i].fB=fl+j;}
                if((fl[i].c==fl[j].a)&&(fl[i].a==fl[j].c)){fl[j].fB=fl+i;fl[i].fB=fl+j;}
                if((fl[i].c==fl[j].c)&&(fl[i].a==fl[j].b)){fl[j].fA=fl+i;fl[i].fB=fl+j;}
        }

}

b_object *b_tetrahedron(char* s)
{
        b_object        *obj;
        b_verlist       verlist,vd;
        b_facelist      facelist;

        obj=A_b_object(s,
        A_b_facelist(4),
        4,A_b_verlist(4),
        4,A_b_verlist(4));


        verlist=obj->verlist;
        vd=obj->verdestlist;
        facelist=obj->facelist;

        verlist[1]=VAL_b_fvertex(100,100,0);
        verlist[0]=VAL_b_fvertex(100,-100,0);
        verlist[2]=VAL_b_fvertex(-100,0,100);
        verlist[3]=VAL_b_fvertex(-100,0,-100);

//        facelist[0]=VAL1_b_face(a,b,c,maptype,tx1,ty1,tx2,ty2,tx3,ty3,texture);
        facelist[0]=VAL1_b_face(vd+0,vd+1,vd+2,0,0,0,0,0.2,0.2,0.2,NULL);
        facelist[1]=VAL1_b_face(vd+0,vd+3,vd+1,0,0,0,0,0.2,0.2,0.2,NULL);
        facelist[2]=VAL1_b_face(vd+0,vd+2,vd+3,0,0,0,0,0.2,0.2,0.2,NULL);
        facelist[3]=VAL1_b_face(vd+1,vd+3,vd+2,0,0,0,0,0.2,0.2,0.2,NULL);

        b_generateneighbourinfo(facelist,obj->numfaces);

        return obj;
}


void    b_inittimer()
{
        cc_addtimer(&enginetimer,"engine");
        cc_addtimer(&transformationtimer,"transformation");
        cc_addtimer(&sorttimer,"sort");
        cc_addtimer(&drawtimer,"draw");
        cc_addtimer(&drawtimer2,"draw2");
}

void    b_init()
{


        {
                int     t;
                for(t=0;t<SORTSIZE;t++)sortt[t]=NULL;
        }
}


b_object *b_findobjectnr(b_object* object, int nr)
{
        b_object        *start=object;

        if(object->keyframenr==nr) return object;
        object=object->next;

        while(object!=start)
        {
                if(object->keyframenr==nr) return object;
                object=object->next;
        }
        // b_error("Object not found:%s\n",s);
        printf("%i",nr);
        b_error("Objectnr not found");
        return object;
}

int     b_getrelativelevel(b_object* obj1, b_object* obj2)
{
        int     level=0;
        b_object *obj=obj1;

        while(obj!=obj2)
        {
                level+=obj->hnext;
                obj=obj->next;
                if(obj==obj1)b_error("b_gtrelativelevel: Objects not in same object-chain");
        }
        return level;
}

void    b_changeobjectlevelrelative(b_object *obj, int level)
{
        obj->hnext-=level;
        obj->next->hprev-=level;
        obj->hprev+=level;
        obj->prev->hnext+=level;
}

void    b_swapobjectpointers(b_object **a, b_object **b)
{
        b_object        *c;
        c=*a;
        *a=*b;
        *b=c;
}

void    b_swapints(int *a, int *b)
{
        int        c;
        c=*a;
        *a=*b;
        *b=c;
}

void    b_exchangeobjects(b_object *a, b_object *b)
{
        b_swapobjectpointers(&a->next,&b->next);
        b_swapobjectpointers(&a->next->prev,&b->next->prev);
        b_swapobjectpointers(&a->prev,&b->prev);
        b_swapobjectpointers(&a->prev->next,&b->prev->next);
        b_swapints(&a->hnext,&b->hnext);
        b_swapints(&a->next->hprev,&b->next->hprev);
        b_swapints(&a->hprev,&b->hprev);
        b_swapints(&a->prev->hnext,&b->prev->hnext);

}
/*
void    b_moveobject(b_object *a, b_object *b)
{
        // inds‘tter object b efter object a

        b_swapojectpointers(&a->next,&b->next);
        b_swapojectpointers(&a->next->prev,&b->next->prev);
        b_swapojectpointers(&a->prev,&b->prev);
        b_swapojectpointers(&a->prev->next,&b->prev->next);
        b_swapints(&a->hnext,&b->hnext);
        b_swapints(&a->next->hprev,&b->next->hprev);
        b_swapints(&a->hprev,&b->hprev);
        b_swapints(&a->prev->hnext,&b->prev->hnext);

}
*/
/*
void    main()
{
        b_scene *scene;
        b_object *a,*b,*c,*d,*e,*f;

        scene=A_b_scene();

        a=A_b_object("A",NULL,0,NULL,0,NULL);
        b=A_b_object("B",NULL,0,NULL,0,NULL);
        c=A_b_object("C",NULL,0,NULL,0,NULL);
        d=A_b_object("D",NULL,0,NULL,0,NULL);
        e=A_b_object("E",NULL,0,NULL,0,NULL);
        f=A_b_object("F",NULL,0,NULL,0,NULL);



        b_insertobject(e,d,1);
        b_insertobject(f,e,1);

        b_insertobject(b,a,1);
        b_insertobject(c,b,1);
        b_insertobject(d,b,1);
        b_printhierarki(a);
        printf("\n");

        b_removeobjecttree(d);

        b_printhierarki(a);
        printf("\n");
        b_printhierarki(d);
        printf("\n");

        b_insertobject(d,b,0);
        b_printhierarki(a);



} */
