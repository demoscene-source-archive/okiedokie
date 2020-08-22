
//Metaballs by Craft/fudGe

//#define pi 3.141592653589793

#include "meta.h"

#define dimx 40
#define dimy 40
#define dimz 40
//#define scale 8
//#define sizescale 2

typedef struct husk{
        int   n;
        fvertex   *v;
        fvertex   *nor;
} husk;

fvertex     *currentface[6];
int     currentvernum=0;
metaball *balls;

int     meta_nf;

fvertex *meta_normals,*meta_vertices;
fvertex *meta_destnormals,*meta_destvertices;
float   meta_f[dimx*dimy*dimz*4];
int     *meta_fi=(int*)meta_f;
b_face    *meta_faces;
int     meta_vertexnum;
int     meta_framenum=0;
husk    meta_husk[dimx*dimy*2*9   ];
int     meta_t1[3][3][3]=
       {{{9,0,9},
         {3,8,5},
         {9,6,9}},
        {{1,2,7},
         {4,9,4},
         {7,2,1}},
        {{9,6,9},
         {5,8,3},
         {9,0,9}}};
int     meta_t2[3][3][3]=
       {{{1,1,1},
         {1,1,1},
         {1,1,1}},
        {{1,1,1},
         {1,9,0},
         {0,0,0}},
        {{0,0,0},
         {0,0,0},
         {0,0,0}}};


static int startx,starty,startz;

float     dist(float a,float b,float c,float x,float y,float z)
{
        return sqrt((float)((a-x)*(a-x)+(b-y)*(b-y)+(c-z)*(c-z)));
}

float     dist2(float a,float b,float c,float x,float y,float z)
{
        return ((a-x)*(a-x)+(b-y)*(b-y)+(c-z)*(c-z));
}

int     meta_test(x,y,z)
{
        return (meta_f[z*dimx*dimy+y*dimx+x]>0);
}

void    swap(int *a,int *b)
{
        int     t;
        t=(*a);
        (*a)=(*b);
        (*b)=t;
}

void   meta_split(int x,int y,int z,int a,int b,int c)
{
        fvertex   ra,rb;
        float     g,h;
        float   j;
        int     p,q;

        int     index;
        int     t1,t2,t3;
        index=meta_t1[x-a+1][y-b+1][z-c+1];
        if(meta_t2[x-a+1][y-b+1][z-c+1])
        {
             /*   swap(&x,&a);
                swap(&y,&b);
                swap(&z,&c);
               */ t1=a;
                t2=b;
                t3=c;
                a=x;
                b=y;
                c=z;
                x=t1;
                y=t2;
                z=t3;
        }

        if(meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+2*dimx*dimy*index].n!=meta_framenum+z)
        {

                p=(z*dimy+y)*dimx+x;
                q=(c*dimy+b)*dimx+a;

        //        if(!(meta_test(x,y,z)^meta_test(a,b,c)))printf("fejl\n");
                g=(float)meta_f[p];
                h=(float)meta_f[q];
                j=(h)/(h-g);
                ra.x=((a+j*(x-a)));
                ra.y=((b+j*(y-b)));
                ra.z=((c+j*(z-c)));
                {
                        float     vx,vy,vz;
                        float     ux,uy,uz;
                        float   l;

                        vx=meta_f[p-1]-meta_f[p+1];
                        vy=meta_f[p-dimx]-meta_f[p+dimx];
                        vz=meta_f[p-(dimx*dimy)]-meta_f[p+(dimx*dimy)];
                        ux=meta_f[q-1]-meta_f[q+1];
                        uy=meta_f[q-dimx]-meta_f[q+dimx];                  //RETTET 30.MARTS 1999!
                        uz=meta_f[q-(dimx*dimy)]-meta_f[q+(dimx*dimy)];

                        rb.x=ux+j*(vx-ux);
                        rb.y=uy+j*(vy-uy);
                        rb.z=uz+j*(vz-uz);

                        l=1/128.0*110/dist(0,0,0,rb.x,rb.y,rb.z);
                        rb.x*=l;
                        rb.y*=l;
                        rb.z*=l;
                }
                meta_vertices[meta_vertexnum].x=ra.x*3*15;
                meta_vertices[meta_vertexnum].y=ra.y*3*15;
                meta_vertices[meta_vertexnum].z=ra.z*3*15;
                meta_normals[meta_vertexnum].x=rb.x;
                meta_normals[meta_vertexnum].y=rb.y;
                meta_normals[meta_vertexnum].z=rb.z;

                meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+dimx*2*dimy*index].n=meta_framenum+z;
                meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+dimx*2*dimy*index].v=meta_destvertices+meta_vertexnum;
                meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+dimx*2*dimy*index].nor=meta_destnormals+meta_vertexnum;
                meta_vertexnum++;
        }
        currentface[currentvernum]=meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+dimx*2*dimy*index].v;
        currentface[currentvernum+3]=meta_husk[(x-startx)+(y-starty)*dimx+((z-startz)&1)*dimx*dimy+dimx*2*dimy*index].nor;
        currentvernum++;
}
void    meta_add()
{
        currentvernum=0;
        meta_faces[meta_nf].a=currentface[0];
        meta_faces[meta_nf].b=currentface[1];
        meta_faces[meta_nf].c=currentface[2];
        meta_faces[meta_nf].na=currentface[3];
        meta_faces[meta_nf].nb=currentface[4];
        meta_faces[meta_nf].nc=currentface[5];
        meta_nf++;
/*        if(meta_n>meta_p+20000)
        {
                printf("ikke nok face-mem\n");
                exit(1);
        }*/

}


void    meta_t(int ax,int ay,int az,int bx,int by,int bz,int cx,int cy,int cz,int dx,int dy,int dz)
{
        fvertex p1,p2,p3;
        int     t1,t2,t3;
        int     as;   //antal swaps

        as=0;

        //sortering s† meta_test(ax,ay,az) <= meta_test(bx,by,bz)
        if (meta_fi[az*dimx*dimy+ay*dimx+ax]<0)
        {
                as=1-as;
                // swap a,b
                t1=ax;
                t2=ay;
                t3=az;
                ax=bx;
                ay=by;
                az=bz;
                bx=t1;
                by=t2;
                bz=t3;
        }
        if (meta_fi[bz*dimx*dimy+by*dimx+bx]<0)
        {
                as=1-as;
                // swap b,c
                t1=cx;
                t2=cy;
                t3=cz;
                cx=bx;
                cy=by;
                cz=bz;
                bx=t1;
                by=t2;
                bz=t3;
        }
        if (meta_fi[cz*dimx*dimy+cy*dimx+cx]<0)
        {
                as=1-as;
                // swap c,d
                t1=cx;
                t2=cy;
                t3=cz;
                cx=dx;
                cy=dy;
                cz=dz;
                dx=t1;
                dy=t2;
                dz=t3;
        }
        if (meta_fi[az*dimx*dimy+ay*dimx+ax]<0)
        {
                as=1-as;
                // swap a,b
                t1=ax;
                t2=ay;
                t3=az;
                ax=bx;
                ay=by;
                az=bz;
                bx=t1;
                by=t2;
                bz=t3;
        }
        if (meta_fi[bz*dimx*dimy+by*dimx+bx]<0)
        {
                as=1-as;
                // swap b,c
                t1=cx;
                t2=cy;
                t3=cz;
                cx=bx;
                cy=by;
                cz=bz;
                bx=t1;
                by=t2;
                bz=t3;
        }
        if (meta_fi[az*dimx*dimy+ay*dimx+ax]<0)
        {
                as=1-as;
                // swap a,b
                t1=ax;
                t2=ay;
                t3=az;
                ax=bx;
                ay=by;
                az=bz;
                bx=t1;
                by=t2;
                bz=t3;
        }

        if (meta_fi[dz*dimx*dimy+dy*dimx+dx]>=0)
        {           //alle punkter inde i objectet
        }
        else

        if (meta_fi[cz*dimx*dimy+cy*dimx+cx]>=0)
        {           //3 punkter inde i objectet
                if (!as)
                {
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_split(dx,dy,dz,bx,by,bz);
                        meta_split(dx,dy,dz,cx,cy,cz);
                        meta_add();
                }
                else
                {
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_split(dx,dy,dz,cx,cy,cz);
                        meta_split(dx,dy,dz,bx,by,bz);
                        meta_add();
                }
        }
        else

        if (meta_fi[bz*dimx*dimy+by*dimx+bx]>=0)
        {           //2 punkter inde i objectet
                if (!as)
                {
                        meta_split(cx,cy,cz,ax,ay,az);
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_split(cx,cy,cz,bx,by,bz);
                        meta_add();
                        meta_split(dx,dy,dz,bx,by,bz);
                        meta_split(cx,cy,cz,bx,by,bz);
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_add();
                }
                else
                {
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_split(cx,cy,cz,ax,ay,az);
                        meta_split(cx,cy,cz,bx,by,bz);
                        meta_add();
                        meta_split(cx,cy,cz,bx,by,bz);
                        meta_split(dx,dy,dz,bx,by,bz);
                        meta_split(dx,dy,dz,ax,ay,az);
                        meta_add();
                }
        }
        else

        if (meta_fi[az*dimx*dimy+ay*dimx+ax]>=0)
        {           //1 punkter inde i objectet

                if (!as)
                {
                        meta_split(ax,ay,az,dx,dy,dz);
                        meta_split(ax,ay,az,bx,by,bz);
                        meta_split(ax,ay,az,cx,cy,cz);
                        meta_add();
                }
                else
                {
                        meta_split(ax,ay,az,bx,by,bz);
                        meta_split(ax,ay,az,dx,dy,dz);
                        meta_split(ax,ay,az,cx,cy,cz);
                        meta_add();
                }
        }
}

static float vaegt;   //0.95
static float vaegtinv;//0.05

void    meta_gridxy(int z)
{
        int     x,y;
        float   *grid;
        metaball *balls2,**bold,*balls3,*balls4;

        grid=z*dimx*dimy+meta_f;

        balls3=balls;

        bold=&balls3;
        balls2=*bold;
        while(balls2)
        {
                balls2->dz=(z-balls2->z)*(z-balls2->z)-balls->size;

                if(balls2->dz>0)*bold=balls2->next1;
                else
                {
                        *bold=balls2;
                        bold=&balls2->next2;
                }
                balls2=balls2->next1;
        }

        for(y=0+starty;y<dimy+starty;y++)
        {
                float xf;

                balls4=balls3;
                bold=&balls4;
                balls2=balls4;
                while(balls2)
                {
                        balls2->dzy=(y-balls2->y)*(y-balls2->y)+balls2->dz;

                        if(balls2->dzy>0)*bold=balls2->next2;
                        else
                        {
                                *bold=balls2;
                                bold=&balls2->next3;
                        }
                        balls2=balls2->next2;
                }
                xf=0;
                for(x=0+startx;x<dimx+startx;x++)
                {
                        grid[x]=grid[x]*vaegt-0.5*vaegtinv;
                }
                {
                        metaball *balls2;

                        balls2=balls4;
                        while(balls2)
                        {
                                int     start,stop;

                                start=balls2->x-balls2->size0;
                                stop=balls2->x+balls2->size0;
//                                if(start<0)start=0;
//                                if(stop>dimx)stop=dimx;

//                                start=startx;
//                                stop=startx+dimx;

                                xf=start;
                                for(x=start;x<stop;x++)
                                {
                                        float d;

                                        d=(balls2->x-xf)*(balls2->x-xf)+balls2->dzy;

                                        if(d<=0)
                                        {
                                                d*=balls2->ssinv;
                                                grid[x]+=(d*d)*(d*d);
                                        }
                                        xf++;
                                }
                                balls2=balls2->next3;
                        }
                        grid+=dimx;
                        xf++;
                }
        }
}


void    meta_grid(void)
{
        int z;
        for(z=0+startz;z<dimz+startz;z++)
        meta_gridxy(z);
}



void    meta_make()
{
        int     x,y,z,a;
        int     p;

        meta_nf=0;

        p=0+startx+starty*dimx+startz*dimx*dimy;

        for(z=0+startz;z<dimz-1+startz;z++)
        {
                for(y=0+starty;y<dimy-1+starty;y++)
                {
                        for(x=0+startx;x<dimx-1+startx;x++)
                        {
                                if((meta_fi[p]&meta_fi[p+1]&meta_fi[p+dimx]&meta_fi[p+1+dimx]
                                 &meta_fi[p+dimx*dimy]&meta_fi[p+dimx*dimy+1]&meta_fi[p+dimx*dimy+dimx]&meta_fi[p+dimx*dimy+dimx+1])>=0)
                                if((meta_fi[p]|meta_fi[p+1]|meta_fi[p+dimx]|meta_fi[p+1+dimx]
                                 |meta_fi[p+dimx*dimy]|meta_fi[p+dimx*dimy+1]|meta_fi[p+dimx*dimy+dimx]|meta_fi[p+dimx*dimy+dimx+1])<0)
                                {
                                        if(((x+y+z)&1)==0)
                                        {
                                                meta_t(x,y,z,
                                                       x+1,y,z,
                                                       x,y+1,z,
                                                       x,y,z+1);
                                                meta_t(x+1,y,z+1,
                                                       x+1,y+1,z+1,
                                                       x+1,y,z,
                                                       x,y,z+1);
                                                meta_t(x,y+1,z+1,
                                                       x,y+1,z,
                                                       x+1,y+1,z+1,
                                                       x,y,z+1);
                                                meta_t(x+1,y+1,z,
                                                       x,y+1,z,
                                                       x+1,y,z,
                                                       x+1,y+1,z+1);
                                                meta_t(x+1,y+1,z+1,
                                                       x,y+1,z,
                                                       x+1,y,z,
                                                       x,y,z+1);
                                        }
                                        else
                                        {
                                                meta_t(x+1,y,z,
                                                       x+1,y+1,z,
                                                       x,y,z,
                                                       x+1,y,z+1);
                                                meta_t(x,y,z+1,
                                                       x,y,z,
                                                       x,y+1,z+1,
                                                       x+1,y,z+1);
                                                meta_t(x+1,y+1,z+1,
                                                       x,y+1,z+1,
                                                       x+1,y+1,z,
                                                       x+1,y,z+1);
                                                meta_t(x,y+1,z,
                                                       x,y,z,
                                                       x+1,y+1,z,
                                                       x,y+1,z+1);
                                                meta_t(x,y+1,z+1,
                                                       x,y,z,
                                                       x+1,y+1,z,
                                                       x+1,y,z+1);
                                        }
                                 }
                                 p++;
                        }
                        p++;


                }
                p+=dimx;
        }
}

void    meta_cleargrid(void)
{
        int     t;

        for(t=0;t<dimx*dimy*dimz;t++)
        meta_f[t]=-0.5;
}

void    meta(b_face *faces, fvertex *vertices, fvertex *destvertices, fvertex *normals, fvertex *destnormals, metaball *balls2, int startx1, int starty1, int startz1, float v)
{
        vaegt=v;
        vaegtinv=1-v;

        startx=startx1;
        starty=starty1;
        startz=startz1;

        balls=balls2;

        meta_faces=faces;
        meta_vertices=vertices;
        meta_normals=normals;
        meta_destvertices=destvertices;
        meta_destnormals=destnormals;

        meta_framenum+=20;

        meta_vertexnum=0;

        meta_grid();

       meta_make();
}
