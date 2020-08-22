/* by craft / fudge */

#include "quat.h"

void    quat_print(quat q)
{
        printf("a: %f x: %f y: %f z: %f\n",q.a,q.x,q.y,q.z);
}

void    quat_printvertex(vertex q)
{
        printf("x: %f y: %f z: %f\n",q.x,q.y,q.z);
}

vertex  quat_vertex(float x, float y, float z)
{
        vertex  v;

        v.x=x;
        v.y=y;
        v.z=z;

        return v;
}


quat    quat_vertextoquat(vertex v)
{
        quat    q;

        q.a=0;
        q.x=v.x;
        q.y=v.y;
        q.z=v.z;

        return q;
}

vertex  quat_quattovertex(quat q)
{
        vertex  v;
        v.x=q.x;
        v.y=q.y;
        v.z=q.z;

        return v;
}

quat    quat_conjugate(quat q)
{
        q.x*=-1;
        q.y*=-1;
        q.z*=-1;

        return q;
}

quat    quat_multiply(quat p, quat q)
{
        quat    r;

        r.a=p.a*q.a-p.x*q.x-p.y*q.y-p.z*q.z;
        r.x=p.a*q.x+p.x*q.a+p.y*q.z-p.z*q.y;
        r.y=p.a*q.y-p.x*q.z+p.y*q.a+p.z*q.x;
        r.z=p.a*q.z+p.x*q.y-p.y*q.x+p.z*q.a;

        return r;
}

quat    quat_trimultiply(quat p, quat q, quat r)
{
        return quat_multiply(p,quat_multiply(q,r));
}

vertex  quat_transform(quat q, vertex v)
{
        quat    p;

        p=quat_conjugate(q);
        return quat_quattovertex(quat_trimultiply(q,quat_vertextoquat(v),p));
}

void    quat_quattomatrix(quat q, fmatrix* m)
/*{
        vertex  v;

        resetmatrixf(m);

        v=quat_transform(q,quat_vertex(1,0,0));
        m->xu=v.x;
        m->yu=v.y;
        m->zu=v.z;
        v=quat_transform(q,quat_vertex(0,1,0));
        m->xv=v.x;
        m->yv=v.y;
        m->zv=v.z;
        v=quat_transform(q,quat_vertex(0,0,1));
        m->xw=v.x;
        m->yw=v.y;
        m->zw=v.z;
}*/
/*{       //Taget fra Watts quatlib.c
        float   s,xs,ys,zs,wx,wy,wz,xx,xy,xz,yy,yz,zz;

        s=2.0/(q.x*q.x+q.y*q.y+q.z*q.z+q.a*q.a);

        xs=q.x*s;
        ys=q.y*s;
        zs=q.z*s;

        wx=q.a*xs;
        wy=q.a*ys;
        wz=q.a*zs;

        xx=q.x*xs;
        xy=q.x*ys;
        xz=q.x*zs;

        yy=q.y*ys;
        yz=q.y*zs;

        zz=q.z*zs;

        m->xu=1.0-(yy+zz);
        m->yu=xy+wz;
        m->zu=xz-wy;

        m->xv=xy-wz;
        m->yv=1.0-(zz+xx);
        m->zv=yz+wx;

        m->xw=xz+wy;
        m->yw=yz-wx;
        m->zw=1.0-(xx+yy);

        m->xx=0;
        m->yy=0;
        m->zz=0;
} */
{       //Optimeret udgave under antagelse af at q er en enheds quaternion
        float   wx,wy,wz,xx,xy,xz,yy,yz,zz;

        q.a*=1.414213562373;   //sqrt(2)
        q.x*=1.414213562373;   //sqrt(2)
        q.y*=1.414213562373;   //sqrt(2)
        q.z*=1.414213562373;   //sqrt(2)

        wx=q.a*q.x;
        wy=q.a*q.y;
        wz=q.a*q.z;

        xx=q.x*q.x;
        xy=q.x*q.y;
        xz=q.x*q.z;

        yy=q.y*q.y;
        yz=q.y*q.z;

        zz=q.z*q.z;

        m->xu=1.0-(yy+zz);
        m->yu=xy+wz;
        m->zu=xz-wy;

        m->xv=xy-wz;
        m->yv=1.0-(zz+xx);
        m->zv=yz+wx;

        m->xw=xz+wy;
        m->yw=yz-wx;
        m->zw=1.0-(xx+yy);

        m->xx=0;
        m->yy=0;
        m->zz=0;
}

quat    quat_makerotatex(float a)
{
        quat    q;

        q.a=cos(a*0.5);
        q.x=sin(a*0.5);
        q.y=0;
        q.z=0;

        return q;
}

quat    quat_makerotatey(float a)
{
        quat    q;

        q.a=cos(a*0.5);
        q.y=sin(a*0.5);
        q.x=0;
        q.z=0;

        return q;
}

quat    quat_makerotatez(float a)
{
        quat    q;

        q.a=cos(a*0.5);
        q.z=sin(a*0.5);
        q.y=0;
        q.x=0;

        return q;
}

quat    quat_makerotate(float x, float y, float z, float a)
{
        quat    q;
        float   l;

        l=1.0/sqrt(x*x+y*y+z*z);

        q.a=cos(a*0.5);
        q.x=l*sin(a*0.5)*x;
        q.y=l*sin(a*0.5)*y;
        q.z=l*sin(a*0.5)*z;

        return q;
}

void    quat_rotatex(quat *q, float a)
{
        *q=quat_multiply(quat_makerotatex(a),*q);
}

void    quat_rotatey(quat *q, float a)
{
        *q=quat_multiply(quat_makerotatey(a),*q);
}

void    quat_rotatez(quat *q, float a)
{
        *q=quat_multiply(quat_makerotatez(a),*q);
}

void    quat_rotate(quat *q, float x, float y, float z, float a)
{
        *q=quat_multiply(quat_makerotate(x,y,z,a),*q);
}

float   quat_dot(quat p, quat q)
{
        return q.a*p.a+q.x*p.x+q.y*p.y+q.z*p.z;
}

quat    quat_mulscalar(quat q, float s)
{
        q.a*=s;
        q.x*=s;
        q.y*=s;
        q.z*=s;

        return q;
}

quat    quat_add(quat q, quat p)
{
        q.a+=p.a;
        q.x+=p.x;
        q.y+=p.y;
        q.z+=p.z;

        return q;
}

quat    quat_slerp(quat p, quat q, float u)
{
        double  w;

        w=(double)quat_dot(p,q);
        if(w<-0.99999) w=pi;
        else if(w>0.99999) w=0;
        else w=acos(w);

        if((w<0.00001)&&(-w<0.00001))
        {
            //    printf("HEJHEJ\n");
                return quat_add(quat_mulscalar(p,1-u),quat_mulscalar(q,u));
        }

        return quat_add(quat_mulscalar(p,sin((1-u)*w)/sin(w)),quat_mulscalar(q,sin(u*w)/sin(w)));
}

quat    quat_exp(quat q)
{
        float l;

        l=sqrt(q.x*q.x+q.y*q.y+q.z*q.z);
        if(l<0.001)
        {
                q.a=1;
                q.x=0;
                q.y=0;
                q.z=0;
                return q;
        }
        q.a=cos(l);
        l=sin(l)/l;
        q.x*=l;
        q.y*=l;
        q.z*=l;

        return q;
}

quat    quat_ln(quat q)
{
        float v;

        if((q.a>0.999)||(q.a<-0.99))
        {
                q.a=0;
                q.x=0;
                q.y=0;
                q.z=0;
                return q;
        }
        v=acos(q.a);
        v/=sin(v);
        q.a=0;
        q.x*=v;
        q.y*=v;
        q.z*=v;

        return q;
}

quat    quat_squad(quat a, quat b, quat c, quat d, float u)
{
/*        printf("left: ");
        quat_print(quat_slerp(a,d,u));
        printf("right: ");
        quat_print(quat_slerp(b,c,u));
*/ //        printf("u:%f ",);
        return quat_slerp(quat_slerp(a,d,u),quat_slerp(b,c,u),2*u*(1-u));

}

quat    quat_quadranglepoint(quat qm,quat q,quat qp)
{
        return
        quat_multiply
        (
                q,
                quat_exp
                (
                        quat_mulscalar
                        (
                                quat_add
                                (
                                        quat_ln
                                        (
                                                quat_multiply
                                                (
                                                        quat_conjugate(q),qp
                                                )
                                        ),
                                        quat_ln
                                        (
                                                quat_multiply
                                                (
                                                        quat_conjugate(q),qm
                                                )
                                        )
                                ),
                                -0.25
                        )
                )
        );
}

#include "qspline.h"

void    quat_test()
{
        quat q,p;
        vertex  v;
        fmatrix m;

        v=quat_vertex(0,0,1);

        q=quat_makerotatex(pi/2);
        quat_print(q);

        quat_printvertex(v);
        v=quat_transform(q,v);
        quat_printvertex(v);

        quat_rotate(&q,1,2,3,8);

        quat_quattomatrix(q,&m);
        printf("Quaternion matrix:\n");
        printmatrixf(&m);

        resetmatrixf(&m);
        rotatematrixxf(&m,pi/2);
        rotatematrixf(&m,1,2,3,8);
        printf("matrix:\n");
        printmatrixf(&m);


        p=quat_makerotatey(pi/3);
        printf("\n");
        quat_print(p);
        printf("\n");
        {
                float t;
                for(t=0;t<=1.0001;t+=0.05)quat_print(quat_slerp(p,q,t));
        }
        printf("\n");
        quat_print(q);
        printf("\n");


        {
                quat    p,q,r;
                quat    a,b,c;
                float   t;

                p=quat_makerotate(1,2,3,8);
                q=quat_makerotate(1,-2,3,8);
                r=quat_makerotate(-1,2,0,7);

                a=quat_quadranglepoint(r,p,q);
                b=quat_quadranglepoint(p,q,r);
                c=quat_quadranglepoint(q,r,p);

                printf("p: ");quat_print(p);
                printf("q: ");quat_print(q);
                printf("r: ");quat_print(r);
                printf("a: ");quat_print(a);
                printf("b: ");quat_print(b);
                printf("c: ");quat_print(c);


                for(t=0;t<=1.0001;t+=0.1)quat_print(quat_squad(p,a,b,q,t));
                printf("\n");
                for(t=0;t<=1.0001;t+=0.1)quat_print(quat_squad(q,b,c,r,t));
                printf("\n");
                for(t=0;t<=1.0001;t+=0.1)quat_print(quat_squad(r,c,a,p,t));
                printf("\n");



        }


        {
                quat    q;
                qspline_key *s;
                float   t;

                q=quat_makerotate(0,0,1,0);

                s=qspline_new();
                qspline_addkey(&s,0,q);
                quat_rotate(&q,1,1,0,1);
                qspline_addkey(&s,1,q);
                quat_rotate(&q,1,1,1,-2);
                qspline_addkey(&s,2,q);


                for(t=-0.5;t<2.5;t+=0.1)
                {
                        printf("t: %f",t);
                        quat_print(qspline_value(s,t));
                }


        }


//        exit(1);
}


