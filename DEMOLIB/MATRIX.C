/* by craft / fudge */

#ifndef matrix34
#define matrix34

#include <math.h>
#include <stdio.h>

typedef struct fmatrix{
	float	xu,xv,xw,xx;
	float	yu,yv,yw,yy;
	float	zu,zv,zw,zz;
} fmatrix;

typedef struct fvertex{
	float	x,y,z;
} fvertex;

void	resetmatrixf(fmatrix *m)
{
	m->xu=m->yv=m->zw=1;
	m->xv=m->xw=m->xx=m->yu=m->yw=m->yy=m->zu=m->zv=m->zz=0;
}

void	scalematrixf(fmatrix *m, float x, float y, float z)
{
	m->xu*=x;
	m->xv*=x;
	m->xw*=x;
	m->xx*=x;
	m->yu*=y;
	m->yv*=y;
	m->yw*=y;
	m->yy*=y;
	m->zu*=z;
	m->zv*=z;
	m->zw*=z;
	m->zz*=z;
}

void	rotatematrixzf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->xu;
	m->xu=t*c-m->yu*s;
	m->yu=t*s+m->yu*c;

	t=m->xv;
	m->xv=t*c-m->yv*s;
	m->yv=t*s+m->yv*c;

	t=m->xw;
	m->xw=t*c-m->yw*s;
	m->yw=t*s+m->yw*c;

	t=m->xx;
	m->xx=t*c-m->yy*s;
	m->yy=t*s+m->yy*c;
}

void	rotatematrixyf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->zu;
	m->zu=t*c-m->xu*s;
	m->xu=t*s+m->xu*c;

	t=m->zv;
	m->zv=t*c-m->xv*s;
	m->xv=t*s+m->xv*c;

	t=m->zw;
	m->zw=t*c-m->xw*s;
	m->xw=t*s+m->xw*c;

	t=m->zz;
	m->zz=t*c-m->xx*s;
	m->xx=t*s+m->xx*c;
}

void	rotatematrixxf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->yu;
	m->yu=t*c-m->zu*s;
	m->zu=t*s+m->zu*c;

	t=m->yv;
	m->yv=t*c-m->zv*s;
	m->zv=t*s+m->zv*c;

	t=m->yw;
	m->yw=t*c-m->zw*s;
	m->zw=t*s+m->zw*c;

	t=m->yy;
	m->yy=t*c-m->zz*s;
	m->zz=t*s+m->zz*c;
}

void	movematrixf(fmatrix *m, float x, float y, float z)
{
	m->xx+=x;
	m->yy+=y;
	m->zz+=z;
}

void	fmovematrixf(fmatrix *m, float x, float y, float z)
{
	m->xx+=(x*(m->xu))+(y*(m->xv))+(z*(m->xw));
	m->yy+=x*m->yu+y*m->yv+z*m->yw;
	m->zz+=x*m->zu+y*m->zv+z*m->zw;
}

void	frotatematrixzf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->xu;
	m->xu=t*c+m->xv*s;
	m->xv=-t*s+m->xv*c;

	t=m->yu;
	m->yu=t*c+m->yv*s;
	m->yv=-t*s+m->yv*c;

	t=m->zu;
	m->zu=t*c+m->zv*s;
	m->zv=-t*s+m->zv*c;
}

void	frotatematrixyf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->xw;
	m->xw=t*c+m->xu*s;
	m->xu=-t*s+m->xu*c;

	t=m->yw;
	m->yw=t*c+m->yu*s;
	m->yu=-t*s+m->yu*c;

	t=m->zw;
	m->zw=t*c+m->zu*s;
	m->zu=-t*s+m->zu*c;
}

void	frotatematrixxf(fmatrix *m, float v)
{
	float	s,c;
	float	t;

        s=sin(v);
        c=cos(v);

	t=m->xv;
	m->xv=t*c+m->xw*s;
	m->xw=-t*s+m->xw*c;

	t=m->yv;
	m->yv=t*c+m->yw*s;
	m->yw=-t*s+m->yw*c;

	t=m->zv;
	m->zv=t*c+m->zw*s;
	m->zw=-t*s+m->zw*c;
}

void    fscalematrixf(fmatrix *m, float x, float y, float z)
{
	m->xu*=x;
	m->yu*=x;
	m->zu*=x;
	m->xv*=y;
	m->yv*=y;
	m->zv*=y;
	m->xw*=z;
	m->yw*=z;
	m->zw*=z;
}

void    fmulmatrixf(fmatrix *m, fmatrix *n)
{
        //m=m*n

        fmatrix temp;

        temp=*m;

        m->xu=temp.xu*n->xu+temp.xv*n->yu+temp.xw*n->zu;
        m->xv=temp.xu*n->xv+temp.xv*n->yv+temp.xw*n->zv;
        m->xw=temp.xu*n->xw+temp.xv*n->yw+temp.xw*n->zw;
        m->xx=temp.xu*n->xx+temp.xv*n->yy+temp.xw*n->zz+temp.xx;

        m->yu=temp.yu*n->xu+temp.yv*n->yu+temp.yw*n->zu;
        m->yv=temp.yu*n->xv+temp.yv*n->yv+temp.yw*n->zv;
        m->yw=temp.yu*n->xw+temp.yv*n->yw+temp.yw*n->zw;
        m->yy=temp.yu*n->xx+temp.yv*n->yy+temp.yw*n->zz+temp.yy;

        m->zu=temp.zu*n->xu+temp.zv*n->yu+temp.zw*n->zu;
        m->zv=temp.zu*n->xv+temp.zv*n->yv+temp.zw*n->zv;
        m->zw=temp.zu*n->xw+temp.zv*n->yw+temp.zw*n->zw;
        m->zz=temp.zu*n->xx+temp.zv*n->yy+temp.zw*n->zz+temp.zz;
}

void    mulmatrixf(fmatrix *m, fmatrix *n)
{
        //m=n*m

        fmatrix temp;

        temp=*m;

        m->xu=n->xu*temp.xu+n->xv*temp.yu+n->xw*temp.zu;
        m->xv=n->xu*temp.xv+n->xv*temp.yv+n->xw*temp.zv;
        m->xw=n->xu*temp.xw+n->xv*temp.yw+n->xw*temp.zw;
        m->xx=n->xu*temp.xx+n->xv*temp.yy+n->xw*temp.zz+n->xx;

        m->yu=n->yu*temp.xu+n->yv*temp.yu+n->yw*temp.zu;
        m->yv=n->yu*temp.xv+n->yv*temp.yv+n->yw*temp.zv;
        m->yw=n->yu*temp.xw+n->yv*temp.yw+n->yw*temp.zw;
        m->yy=n->yu*temp.xx+n->yv*temp.yy+n->yw*temp.zz+n->yy;

        m->zu=n->zu*temp.xu+n->zv*temp.yu+n->zw*temp.zu;
        m->zv=n->zu*temp.xv+n->zv*temp.yv+n->zw*temp.zv;
        m->zw=n->zu*temp.xw+n->zv*temp.yw+n->zw*temp.zw;
        m->zz=n->zu*temp.xx+n->zv*temp.yy+n->zw*temp.zz+n->zz;
}


void	rotatematrixf(fmatrix *m, float x, float y, float z, float v)
{
	float	i,j;

        i=-atan2(y,x);
        j=atan2(z,sqrt(x*x+y*y));

	rotatematrixzf(m,i);
	rotatematrixyf(m,j);
	rotatematrixxf(m,v);
	rotatematrixyf(m,-j);
	rotatematrixzf(m,-i);
}

void	printmatrixf(fmatrix *m)
{
	printf("%4.4f %4.4f %4.4f %6.2f\n",m->xu,m->xv,m->xw,m->xx);
	printf("%4.4f %4.4f %4.4f %6.2f\n",m->yu,m->yv,m->yw,m->yy);
	printf("%4.4f %4.4f %4.4f %6.2f\n",m->zu,m->zv,m->zw,m->zz);
}

void    transformfasm(fmatrix *m, fvertex *s, fvertex *d, int n);
#pragma aux transformfasm=\
"       cmp     ecx,0                   "\
"       je      ud                      "\
"@@loop:                                "\
"       fld     dword ptr[esi]          "\
"       fmul    dword ptr[edx+0]        "\
"       fld     dword ptr[esi+4]        "\
"       fmul    dword ptr[edx+4]        "\
"       fxch    st(1)                   "\
"       fadd    dword ptr[edx+12]       "\
"       fld     dword ptr[esi+8]        "\
"       fmul    dword ptr[edx+8]        "\
"       fxch    st(2)                   "\
"       fadd                            "\
"       fld     dword ptr[esi]          "\
"       fmul    dword ptr[edx+16]        "\
"       fxch    st(2)                   "\
"       fadd                            "\
"       fxch    st(1)                   "\
"       fld     dword ptr[esi+4]        "\
"       fmul    dword ptr[edx+20]        "\
"       fxch    st(1)                   "\
"       fadd    dword ptr[edx+28]       "\
"       fld     dword ptr[esi+8]        "\
"       fmul    dword ptr[edx+24]        "\
"       fxch    st(2)                   "\
"       fadd                            "\
"       fld     dword ptr[esi]          "\
"       fmul    dword ptr[edx+32]        "\
"       fxch    st(2)                   "\
"       fadd                            "\
"       fxch    st(1)                   "\
"       fld     dword ptr[esi+4]        "\
"       fmul    dword ptr[edx+36]        "\
"       fxch    st(1)                   "\
"       fadd    dword ptr[edx+44]       "\
"       fld     dword ptr[esi+8]        "\
"       fmul    dword ptr[edx+40]        "\
"       fxch    st(2)                   "\
"       fadd                            "\
"       fxch    st(2)                   "\
"       fstp    dword ptr[edi+4]        "\
"       fadd                            "\
"       fxch    st(1)                   "\
"       fstp    dword ptr[edi]          "\
"       fstp    dword ptr[edi+8]        "\
"       add     esi,12                  "\
"       add     edi,12                  "\
"       dec     ecx                     "\
"       jne     @@loop                  "\
"ud:                                    "\
parm[edx][esi][edi][ecx]\
modify[ecx edi esi 8087];

void    transformf(fmatrix *m, fvertex *s, fvertex *d, int n)
{
	int	t;

        transformfasm(m,s,d,n);
/*        for(t=0;t<n;t++)
	{
		d[t].x=m->xu*s[t].x+m->xv*s[t].y+m->xw*s[t].z+m->xx;
		d[t].y=m->yu*s[t].x+m->yv*s[t].y+m->yw*s[t].z+m->yy;
		d[t].z=m->zu*s[t].x+m->zv*s[t].y+m->zw*s[t].z+m->zz;
	}
  */

}


#endif
