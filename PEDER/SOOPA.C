/* vipa / purple */
/*                     Purple intro for the gathering 99
public demand
cone
craqsh
brainman
spacepussy
sunrise
psychopod
dragonfly
departure
enlightened evolution
voyager
photon
*/

#include        "..\main.h"
#include        "..\scriptde.h"
#include        "..\demolib\matrix.h"
#include        "..\zb\zbasm.h"
#include        "..\zb\zoomblur.h"
#include        "..\demolib\layer.h"
#include        "..\demolib\pcx.h"
#include        <math.h>
#include        <string.h>
#include        <malloc.h>
#include        <stdlib.h>

#define byte unsigned char
#define word unsigned short
#define dword unsigned long
int ZMAXTEST=65536;
int SCREENX=320-10;
int SCREENY=200-4;
int sizex,sizey;
float scalex,scaley;
int MIDX=160;
int MIDY=100;

int maxantalpartikler=1000;
int numpartikler=0;

typedef struct{
  float x,y,z;
  float vx,vy,vz;
  float lifespan;
  int col;
} partikel;


#include "tinymath.h"

#include "wire1.inc"
#include "rundkas.inc"

static float zposadd;

//screen variables
#define scr ((char*)0xa0000)
char *vesascr;
int lfb;      //linearframebuffer enabled
int screenbpp;  //screen bpp writemode
unsigned int vdscr[64000];
unsigned int vdscr2[64000];
unsigned int tiletexture[65536];
unsigned int truetexture[65536];
char texture[65536];
char pal[65536];
//dword flare[65536*2];
int pcol[10000];
float xp[10000];
float yp[10000];
int zp[10000];
/*
typedef union {
  unsigned long col;
  struct {
   unsigned short b:8;
   unsigned short g:8;
   unsigned short r:8;
   unsigned short d:8;
  } part;
} realcol;


dword wtexture[85536];
dword wtexture2[85536];
dword wscr2[85536];
dword partik[65536*2];
char  texturett[65536*2];
char  texture2[65536];
  */

#include "metagen.h"
#include "softt.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


static byte wu_table_r[256];
static byte wu_table_g[256];
static byte wu_table_b[256];
static int  wu_table[256];

static int _xinc, _yinc, _slope;


void set_wu_color(float r, float g, float b) {  // 0..1
    int i;
    for(i=0; i<256; i++)
        wu_table_r[i] = min(r*i,255);
    for(i=0; i<256; i++)
        wu_table_g[i] = min(g*i,255);
    for(i=0; i<256; i++)
        wu_table_b[i] = min(b*i,255);
    for(i=0; i<256; i++)
        {
                wu_table[i]=(65536*wu_table_b[i]+256*wu_table_g[i]+wu_table_r[i])&0x00fefefe;
        }
}

void memset32(void *, int, int);
#pragma aux memset32 =\
   "pusha",\
   "mov ebx,ecx",\
   "shr ecx,2",\
   "shl ecx,2",\
   "sub ebx,ecx",\
   "shr ecx,2",\
   "rep stosd",\
   "mov ecx,ebx",\
   "rep stosb",\
   "popa",\
parm [edi] [eax] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void asm_wuline32addcliphurtig(unsigned int *dest, dword prestep, int deltax);
#pragma aux asm_wuline32addcliphurtig parm [edi][edx][ecx] modify [eax ebx esi] =\
    "   push ebp                        "\
    "   mov  ebp, ecx                   "\
    "   xor  ecx, ecx                   "\
    "   mov  esi, [_yinc]               "\
    "start:                             "\
    "   push edx"\
    "   shr  edx,24                     "\
    "   mov  eax, [edi+esi]             "\
    "   mov  ebx, [wu_table+edx*4]      "\
    "   not  dl                         "\
    "   and  eax,0x00fefefe             "\
    "   add  eax,ebx                    "\
    "   mov  ebx,eax                    "\
    "   and  ebx,0x01010100             "\
    "   mov  ecx,ebx                    "\
    "   shr  ebx,8                      "\
    "   sub  ecx,ebx                    "\
    "   or   ecx,eax                    "\
    "   mov  eax, [edi]                 "\
    "   mov  [edi+esi],ecx              "\
    "   mov  ebx, [wu_table+edx*4]      "\
    "   and  eax,0x00fefefe             "\
    "   add  eax,ebx                    "\
    "   mov  ebx,eax                    "\
    "   and  ebx,0x01010100             "\
    "   mov  ecx,ebx                    "\
    "   shr  ebx,8                      "\
    "   sub  ecx,ebx                    "\
    "   or   eax,ecx                    "\
    "   mov  [edi],eax                  "\
    "   pop  edx"\
    "   dec  ebp                        "\
    "   js   done                       "\
    "   add  edi, [_xinc]               "\
    "   add  edx, [_slope]              "\
    "   sbb  ecx,ecx                    "\
    "   and  ecx,esi                    "\
    "   add  edi, ecx                   "\
    "   jmp  start                      "\
    "done:                              "\
    "   pop  ebp                        ";

void asm_wuline32addclip(unsigned int *dest, dword prestep, int deltax);
#pragma aux asm_wuline32addclip parm [edi][edx][ecx] modify [eax ebx esi] =\
    "   push ebp                        "\
    "   mov  ebp, ecx                   "\
    "   xor  ecx, ecx                   "\
    "   mov  esi, [_yinc]               "\
    "start:                             "\
    "   ror  edx, 16                    "\
    "   mov  cl,  dh                    "\
    "   ror  edx, 16                    "\
    "   mov  eax, [edi+esi]             "\
    "   mov  bl,  [wu_table_b+ecx]      "\
    "   add  al,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   al,  bh                    "\
    "   mov  bl,  [wu_table_g+ecx]      "\
    "   add  ah,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   ah,  bh                    "\
    "   ror  eax, 16                    "\
    "   mov  bl,  [wu_table_r+ecx]      "\
    "   add  al,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   al,  bh                    "\
    "   ror  eax, 16                    "\
    "   mov  [edi+esi],eax              "\
    "   mov  eax, [edi]                 "\
    "   not  cl                         "\
    "   mov  bl,  [wu_table_b+ecx]      "\
    "   add  al,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   al,  bh                    "\
    "   mov  bl,  [wu_table_g+ecx]      "\
    "   add  ah,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   ah,  bh                    "\
    "   ror  eax, 16                    "\
    "   mov  bl,  [wu_table_r+ecx]      "\
    "   add  al,  bl                    "\
    "   sbb  bh,  bh                    "\
    "   or   al,  bh                    "\
    "   ror  eax, 16                    "\
    "   mov  [edi],eax                  "\
    "   dec  ebp                        "\
    "   js   done                       "\
    "   add  edi, [_xinc]               "\
    "   add  edx, [_slope]              "\
    "   jnc  start                      "\
    "   add  edi, esi                   "\
    "   jmp  start                      "\
    "done:                              "\
    "   pop  ebp                        ";

#define VISIBLE 0
#define TOP     1
#define BOTTOM  2
#define LLEFT   4
#define RRIGHT  8

int abs(int x)
{
    if (x<0) x=-x;
    return(x);
}


void wuline32addclip(float x1, float y1, float x2, float y2, unsigned int* screen) {
    float deltax, deltay,ftemp;
    int deltaxi, deltayi;
    int xinc, yinc,itemp;
    float slope;
    dword slopei;
    int x2i;
    int y2i;
    int x1i;
    float y1cor;
    int y1i;
    float prestep;
    double a,k;

    int xsize = 320;
    int ysize = 200;

    int p1=VISIBLE, p2=VISIBLE;
    if(x1<1)                p1 |= LLEFT;
    if(x1>xsize-2)   p1 |= RRIGHT;
    if(x2<1)                p2 |= LLEFT;
    if(x2>xsize-2)   p2 |= RRIGHT;
    if((p1&LLEFT) && (p2&LLEFT)) return;
    if((p1&RRIGHT) && (p2&RRIGHT)) return;

    if((p1&(LLEFT|RRIGHT)) || (p2&(LLEFT|RRIGHT))) {
        a = (double)(y2-y1)/(x2-x1);
        k = y1-a*x1;
        x1 = min(max(x1,1),xsize-2);
        y1 = a*x1+k;
        x2 = min(max(x2,1),xsize-2);
        y2 = a*x2+k;
    }

    if(y1<1)                 p1 |= TOP;
    if(y1>ysize-2)    p1 |= BOTTOM;
    if(y2<1)                 p2 |= TOP;
    if(y2>ysize-2)    p2 |= BOTTOM;
    if((p1&TOP) && (p2&TOP)) return;
    if((p1&BOTTOM) && (p2&BOTTOM)) return;

    if((p1&(TOP|BOTTOM)) || (p2&(TOP|BOTTOM))) {
        a = (double)(x2-x1)/(y2-y1);
        k = x1-a*y1;
        y1 = min(max(y1,1),ysize-2);
        x1 = min(max(a*y1+k,1),xsize-2);
        y2 = min(max(y2,1),ysize-2);
        x2 = min(max(a*y2+k,1),xsize-2);
    }

    deltax = x2-x1;
    deltay = y2-y1;

    if(deltax > 0) {
        xinc = 4;
    } else {
        xinc = -4;
        x1 = -x1;
        x2 = -x2;
    }
    deltax = fabs(deltax);
    if(deltay > 0) {
        yinc = 4*xsize;
    } else {
        yinc = -4*xsize;
        y1 = -y1;
        y2 = -y2;
    }
    deltay = fabs(deltay);
    if(deltay>deltax) {
        ftemp=deltax;
        deltax=deltay;
        deltay=ftemp;
        itemp=xinc;
        xinc=yinc;
        yinc=itemp;
        ftemp=x1;
        x1=y1;
        y1=ftemp;
        ftemp=x2;
        x2=y2;
        y2=ftemp;
        itemp=xsize;
        xsize=ysize;
        ysize=itemp;
/*        exchange(deltax,deltay);
        exchange(xinc,yinc);
        exchange(x1,y1);
        exchange(x2,y2);
        exchange(xsize,ysize);*/
    }
    slope = 0;
    slopei = 0;
    if(deltax) {
        slope = (float)(deltay)/(float)(deltax);
        slopei = min(4294967296.0*slope, 4294967295.0);
    }
    x2i = x2;
    y2i = y2;
    x1i = x1;
//    if ((x2i-x1i)>0) deltaxi=x2i-x1i; else deltaxi=-(x2i-x1i);
    deltaxi = abs(x2i-x1i);

    while(deltaxi>=0) {
        float xerror = x1i - x1;
        y1cor = y1 + xerror*slope;
        y1i = y1cor;
        if(y1i<0) y1i--;
//        if( (  ( ((y1i)>=1) || ((-y1i)>=1)) ) &&   ((((y1i)>0)<=(ysize-2)) || (((-y1i)>0)<=(ysize-2))) ) break;
        if((abs(y1i)>=1) && (abs(y1i)<=(ysize-2))) break;
        x1i += 1;
        deltaxi--;
    }
    if(deltaxi<0) return;

    prestep = fmod(y1cor,1);
    prestep *= 4294967296.0;

 //   byte *dest = (byte*)screen + y1i*yinc + x1i*xinc;
//    byte *dest = (byte*)screen + y1i*yinc + x1i*xinc;

    _xinc = xinc;
    _yinc = yinc;
    _slope = slopei;
    asm_wuline32addcliphurtig(vdscr+ (y1i*yinc + x1i*xinc)/4,prestep,deltaxi);
//    asm_wuline32addclip(vdscr+ (y1i*yinc + x1i*xinc)/4,prestep,deltaxi);
}


void generategridplane(int xd,int yd,int zd,int nump,int numf,int boxsize,int flip,int scale)
{ int i,j;

    for (i=0;i<boxsize;i++)
    for (j=0;j<boxsize;j++) {
        if (xd!=0) points2[(i*boxsize+j+nump)*3+0]=(xd*(boxsize-1)/2.0)*scale;
        if (xd==0) points2[(i*boxsize+j+nump)*3+0]=(j-(boxsize-1)/2.0)*scale ;
        if (yd!=0) points2[(i*boxsize+j+nump)*3+1]=(yd*(boxsize-1)/2.0)*scale;
        if (yd==0) points2[(i*boxsize+j+nump)*3+1]=(i-(boxsize-1)/2.0)*scale;
        if (zd!=0) points2[(i*boxsize+j+nump)*3+2]=(zd*(boxsize-1)/2.0)*scale;
        if ((zd==0) && (yd!=0)) points2[(i*boxsize+j+nump)*3+2]=(i-(boxsize-1)/2.0)*scale;
        if ((zd==0) && (yd==0)) points2[(i*boxsize+j+nump)*3+2]=(j-(boxsize-1)/2.0)*scale;

        numpoints++;
    }
    for (i=0;i<boxsize-1;i++)
    for (j=0;j<boxsize-1;j++) {
        faces[numfaces*3+(0*flip)%3]=i*boxsize+j+nump;
        faces[numfaces*3+(1*flip)%3]=i*boxsize+j+1+nump;
        faces[numfaces*3+(2*flip)%3]=(i+1)*boxsize+j+nump;
        numfaces+=1;
        faces[numfaces*3+(0*flip)%3]=i*boxsize+j+1+nump;
        faces[numfaces*3+(1*flip)%3]=(i+1)*boxsize+j+1+nump;
        faces[numfaces*3+(2*flip)%3]=(i+1)*boxsize+j+nump;
        numfaces+=1;
    }
}

float pointsbak[40024*3];

void blob(int doinit, int useframe)
{   static int sub2,sub,temp,test,i,j,a,b,col,zbufcount,c,u,v,dst,testinc,boxsize;
   static word zbuf;
    static float x,y,z,ppx,ppy,ppz,phiz=0,phiy=0,phix=0,tanfov,splength,radius,scale;
    static float kuglecx,kuglecy,kuglecz;
    static int xpm, ypm, zpm, dup,nl;

   if (doinit) {
   for (a=0;a<256;a++)
   for (b=0;b<256;b++)
   texture[a*256+b]=fexp(((181-fsqrt((a-128)*(a-128)+(b-128)*(b-128)))*1.4)/46.0);

   for (a=0;a<256;a++)
   for (b=0;b<256;b++)
   texture[a*256+b]=(((181-fsqrt((a-128)*(a-128)+(b-128)*(b-128)))*1.4)+texture[a*256+b])/2;

       for(v=0; v<256; v++) {
           for(u=0; u<256; u++) {
               dst = ((u<<8) & 0xf800)+(u & 0x0007)+((v<<3) & 0x07f8);
               tiletexture[dst] =
               (int)(texture[u+v*256]*0.94)+
               (int)(texture[u+v*256]*0.66)*256+
               (int)(texture[u+v*256]*0.1)*65536;
              }
          }
    boxsize=20;
    scale=15;
    numfaces=rk_numfaces;
    numpoints=rk_numpoints;
    for (i=0;i<numpoints*3;i++) points2[i]=rk_points[i];
    for (i=0;i<numfaces*3;i++) faces[i]=rk_faces[i];
/*    numpoints=0;
    numfaces=0;
    generategridplane(0,0,1,numpoints,numfaces,boxsize,1,scale);
    generategridplane(0,0,-1,numpoints,numfaces,boxsize,2,scale);
    generategridplane(0,-1,0,numpoints,numfaces,boxsize,1,scale);
    generategridplane(0,1,0,numpoints,numfaces,boxsize,2,scale);
    generategridplane(-1,0,0,numpoints,numfaces,boxsize,1,scale);
    generategridplane(1,0,0,numpoints,numfaces,boxsize,2,scale);*/
    kuglecx=0;
    kuglecy=0;
    kuglecz=0;
    radius=200;
    memcpy(&pointsbak,&points2,numpoints*3*4);
    for (i=0;i<numpoints;i++) {
        x=points2[i*3+0]-kuglecx;
        y=points2[i*3+1]-kuglecy;
        z=points2[i*3+2]-kuglecz;
        splength=fsqrt(x*x+y*y+z*z);
        if (splength<radius) {
          points2[i*3+0]=points2[i*3+0]*radius/splength;
          points2[i*3+1]=points2[i*3+1]*radius/splength;
          points2[i*3+2]=points2[i*3+2]*radius/splength;
        }
    }

    test=0;
    tanfov = 0.70;
    sizex=tanfov*SCREENX / 2 / tanfov;
    sizey=tanfov*1.2*SCREENY / 2 / 1.2/ tanfov;

    scalex=tanfov;
    scaley=tanfov*1.2*1.18;

    camera1_3ds.position.x=-190;
    camera1_3ds.position.y=2;
    camera1_3ds.position.z=1500;
    camera1_3ds.target.x=0;
    camera1_3ds.target.y=0;
    camera1_3ds.target.z=0;
    camera1_3ds.roll=-3.1415/2;
    test=0;
//    framenum=0;
    }
    else

//    while ((!kbhit()) &&(test<3000))
    {
    camera1_3ds.position.z=550+sin(phix*1)*100;
    createrotatematrix(rotmat,phix,phiy,phiz);
    size=100; //no use
    zpos=100;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++) {tempmat[i][j]=0;}
    for (i = 0; i < 4; i++) tempmat[i][i]=1;              //create unimatrix
    tempmat[0][0]=scalex;
    tempmat[1][1]=scaley;
    tempmat[2][2]=1;
    matmult(rotmat,tempmat);
    dup=0;

    for (nl=0;nl<numfaces;nl++)   //numbers of vertexs.
        {
        face2[nl].v1=rotatelist+faces[3*nl];
        face2[nl].v2=rotatelist+faces[3*nl+1];
        face2[nl].v3=rotatelist+faces[3*nl+2];
        }
    memcpy(&points2,&pointsbak,numpoints*3*4);
    kuglecx=fsin(phix)*130;
    kuglecy=fsin(phiy)*130;
    kuglecz=fsin(phiz)*130;
    radius=160;
    for (i=0;i<numpoints;i++) {
        x=points2[i*3+0]-kuglecx;
        y=points2[i*3+1]-kuglecy;
        z=points2[i*3+2]-kuglecz;
        splength=fsqrt(x*x+y*y+z*z);
        if (splength<radius) {
          points2[i*3+0]=(points2[i*3+0]-kuglecx)*radius/splength+kuglecx;
          points2[i*3+1]=(points2[i*3+1]-kuglecy)*radius/splength+kuglecy;
          points2[i*3+2]=(points2[i*3+2]-kuglecz)*radius/splength+kuglecz;
        }
/*        if (splength>radius) {
          points2[i*3+0]=(points2[i*3+0]-kuglecx)*radius/splength+kuglecx;
          points2[i*3+1]=(points2[i*3+1]-kuglecy)*radius/splength+kuglecy;
          points2[i*3+2]=(points2[i*3+2]-kuglecz)*radius/splength+kuglecz;
        }*/
    }
    kuglecx=fsin(phix*1.7)*120;
    kuglecy=fsin(phiy*1.3)*120;
    kuglecz=fsin(phiz*1.2)*120;
    radius=160;
    for (i=0;i<numpoints;i++) {
        x=points2[i*3+0]-kuglecx;
        y=points2[i*3+1]-kuglecy;
        z=points2[i*3+2]-kuglecz;
        splength=fsqrt(x*x+y*y+z*z);
        if (splength<radius) {
          points2[i*3+0]=(points2[i*3+0]-kuglecx)*radius/splength+kuglecx;
          points2[i*3+1]=(points2[i*3+1]-kuglecy)*radius/splength+kuglecy;
          points2[i*3+2]=(points2[i*3+2]-kuglecz)*radius/splength+kuglecz;
        }
    }
    kuglecx=fsin(phix*1.9)*120;
    kuglecy=fsin(phiy*1.7)*120;
    kuglecz=fsin(phiz*1.3)*120;
    radius=160;
    for (i=0;i<numpoints;i++) {
        x=points2[i*3+0]-kuglecx;
        y=points2[i*3+1]-kuglecy;
        z=points2[i*3+2]-kuglecz;
        splength=fsqrt(x*x+y*y+z*z);
        if (splength<radius) {
          points2[i*3+0]=(points2[i*3+0]-kuglecx)*radius/splength+kuglecx;
          points2[i*3+1]=(points2[i*3+1]-kuglecy)*radius/splength+kuglecy;
          points2[i*3+2]=(points2[i*3+2]-kuglecz)*radius/splength+kuglecz;
        }
    }

    rotateobject();
    fastcalcnormals();
    fastcalcfakenormals();
    for (i=0;i<numpoints;i++) {
        rotatelist[i].u=((fnormals[i*3+0]/8.0))+128;
        rotatelist[i].v=((fnormals[i*3+1]/8.0))+128;
//        rotatelist[i].u=128;
//        rotatelist[i].v=128;
    }
    i=0;
	while (i<numfaces)
	{
        polyz[i]=(face2[i].v1->z+face2[i].v2->z+face2[i].v3->z);
		pind[i]=i;
		i++;
    }
     quicksort(0,numfaces-1);
//   rblur(160,100,64000);
//     vesacopy(vdscr);
//     memset32(vdscr,255*65536+(int)(0.6*256)*256+(int)(0.1*256),64000*4);
     memset32(vdscr,(int)(0.95*255)*65536+(int)(0.66*255)*256+(int)(0.03*255),64000*4);
//     memset32(vdscr,0,64000*4);

      for (a=0;a<numfaces;a++) {
      i=pind[numfaces-a-1];
      if (!(face2[i].v1->cb & face2[i].v2->cb & face2[i].v3->cb)){
        if (( (face2[i].v3->x*((face2[i].v1->z*face2[i].v2->y)-(face2[i].v1->y*face2[i].v2->z)))+
              (face2[i].v3->y*((face2[i].v1->x*face2[i].v2->z)-(face2[i].v1->z*face2[i].v2->x)))+
              (face2[i].v3->z*((face2[i].v1->y*face2[i].v2->x)-(face2[i].v1->x*face2[i].v2->y))) )>0) {

        face2[i].clip= (face2[i].v1->cb | face2[i].v2->cb | face2[i].v3->cb);
        drawpolytype=face2[i].polytype;
        polycolor=face2[i].color;
        if(face2[i].clip) clipright(&face2[i]);
        else
        renderpoly(&face2[i]);

      }}
      }
      while (useframe>0) {
        phix+=0.01757;
        phiy+=0.01538;
        phiz+=0.00224;
       if (testinc==1) test++;
       if (testinc==0) test--;
       if (test<1) testinc=1;
       if (test>30) testinc=0;
       test=0;
       pos+=1;
//       framenum++;
       useframe--;
      }
    }
}


float sintab[1024];
int pointsbal[8000*3];
rgb1* backpicwire;
rgb1* weknows;
unsigned char shakepic[70000];
unsigned char weknowsfinal[64000];

void weknowsshake(int doinit,int useframe)
{   int i,pos,j;
        static int framenum;
        static int nextframe;


    if (doinit) {
     memset(shakepic,0,70000);
     weknows=pcx_read_picture("peder\\wkelvis.pcx",1);
     for (i=0;i<64000;i++) shakepic[i]=(unsigned char)(weknows[i].r)/15;
    } else {

        framenum+=useframe;
        if(framenum>=nextframe)
        {
    memset(weknowsfinal,0,64000);
    pos=0;
    for (j=0;j<15;j++) {
        pos+=(rand()%2)+(rand()%2)*320;
        for (i=0;i<64000/4;i++) ((int*)weknowsfinal)[i]+=((int*)(shakepic+pos))[i];
    }
    for (i=0;i<64000;i++) vdscr[i]=weknowsfinal[i]+weknowsfinal[i]*256+weknowsfinal[i]*65536;
        nextframe+=3;
        }
    }
}

void wire(int doinit,int useframe)
{   static int slength,xpp,ypp,du,dv,u,v,fadecol,ddl,ddl2,lll,lll2,ll,dl,dest,lxx,lyy,lxx2,lyy2,xx2,yy2,ddx2,ddy2,ddx,ddy,dx,dy,xx,yy,action,zpos,pos1,a,b,zadd,rndx,rndy,rndz,zheight,effectnum,xr,yr,test;
    static float vdu,vdv,c1,c2,c3,c4,zhorizon,invdu,invdv,w1,w2,w3,w4;
    static float phix,phiy,phiz,x1,y1,x2,y2,x3,y3,x,y,z;
    static float planz,dd,ppx,ppy,ppz,size,randfunc2;
    static float dcol,z1;
    static int rnd,pnum,col,pfade,i,randfunc,xpos,ypos;

    if (doinit) {
    backpicwire=pcx_read_picture("peder\\creds_b.pcx",1);

    numfaces=ba_numfaces;
    numpoints=ba_numpoints;
    for (i=0;i<numpoints*3;i++) pointsbal[i]=ba_points[i];
    for (i=0;i<numfaces*3;i++) faces[i]=ba_faces[i];

    phix=0;
    phiy=0;
    pnum=0;
    phiz=3.14;
    zpos=4000;
    size=256;
    zadd=8;
    action=0;
    fadecol=600000;
    zheight=0;
    effectnum=1;
    pos1=100;
    pfade=128;
    size=64;
    test=0;
    dcol=0.0;
    randfunc=1;
    randfunc2=1/randfunc;
    xpos=160;
    ypos=100;
    for (i=0;i<1024;i++) sintab[i]=fsin(i);
    }
     else {
//        memset(vdscr,40,64000*4);
        memcpy(vdscr,backpicwire,64000*4);
        set_wu_color(dcol,dcol,dcol);
        createrotatematrix(rotmat,phix,phiy,phiz);
        i=0;
        while (i<numpoints)
        {
//          ppx=(points[i+i+i]  *(fsin(points[i+i+i]/128.0+phiz*5)  +10))/10;
//          ppy=(points[i+i+i+1]*(fsin(points[i+i+i+1]/128.0+phiz*7)+10))/10;
//          ppz=(points[i+i+i+2]*(fsin(points[i+i+i+2]/128.0+phiz*6)+10))/10;
            ppx=(pointsbal[i+i+i  ]*(sintab[(pointsbal[i+i+i  ])&1023]+randfunc))*randfunc2;
            ppy=(pointsbal[i+i+i+1]*(sintab[(pointsbal[i+i+i+1])&1023]+randfunc))*randfunc2;
            ppz=(pointsbal[i+i+i+2]*(sintab[(pointsbal[i+i+i+2])&1023]+randfunc))*randfunc2;
//            ppx=(points[i+i+i]   *(fsin(points[i+i+i])+randfunc))/randfunc;
//            ppy=(points[i+i+i+1] *(fsin(points[i+i+i+1])+randfunc))/randfunc;
//            ppz=(points[i+i+i+2] *(fsin(points[i+i+i+2])+randfunc))/randfunc;
//            ppx=points[i+i+i];
//            ppy=points[i+i+i+1];
//            ppz=points[i+i+i+2];
            x = (ppx*rotmat[0][0]+ppy*rotmat[1][0]+ppz*rotmat[2][0]);
            y = (ppx*rotmat[0][1]+ppy*rotmat[1][1]+ppz*rotmat[2][1]);
            z = (ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2])-zpos+zposadd;
            xp[i]=x;
            yp[i]=y;
            zp[i]=z;
            i++;
        }
      for (i=0;i<numfaces;i++) {
         if ((zp[faces[i*3]]<0) && (zp[faces[i*3+1]]<0) && (zp[faces[i*3+2]]<0))
         {
              z1=size/zp[faces[i*3]];
              x1=(xpos+(float)(xp[faces[i*3]]*z1));
              y1=(ypos+(float)(yp[faces[i*3]]*z1));
              z1=size/zp[faces[i*3+1]];
              x2=(xpos+(float)(xp[faces[i*3+1]]*z1));
              y2=(ypos+(float)(yp[faces[i*3+1]]*z1));
              z1=size/zp[faces[i*3+2]];
              x3=(xpos+(float)(xp[faces[i*3+2]]*z1));
              y3=(ypos+(float)(yp[faces[i*3+2]]*z1));
             wuline32addclip(x1,y1,x2,y2,vdscr);
//             wuline32addclip(x1,y1,x3,y3,vdscr);

//           wuline8addclip(x2,y2,x3,y3,v8scr);
//             x1=xp[faces[i*3+1]];y1=yp[faces[i*3+1]];x2=xp[faces[i*3+2]];y2=yp[faces[i*3+2]];
//             wuline8addclip(x1,y1,x2,y2,vscr);
//             x1=xp[faces[i*3+2]];y1=yp[faces[i*3+2]];x2=xp[faces[i*3+0]];y2=yp[faces[i*3+0]];
//             wuline8addclip(x1,y1,x2,y2,vscr);

         }
      }
//     if ((rand()%100)==0) randfunc=2;
//     if (randfunc<1024) randfunc+=1;
//     randfunc2=1.0/randfunc;
    while (useframe>0) {
      if (dcol<0.6) dcol+=0.0002;
      if (zpos>900) zpos-=3;
      phix+=0.0035*sw[SW_WIREROTX]/10.0;
      phiy+=0.0043*sw[SW_WIREROTY]/10.0;
      phiz+=0.0053*sw[SW_WIREROTZ]/10.0;
      useframe--;
        }
    }
}



void wire_init2()
{

}
void wire_init1()
{
  face2=(FacelistPL*)malloc(sizeof(FacelistPL)*20000);
  wire(1,0);
}

void wire_deinit()
{
//        free(face2);
}


layer wire_main(int useframe)
{
        zposadd-=(float)sw[SW_WIREZOOM]*useframe;
        wire(0,useframe);
        return layer_make(vdscr,0,0,320,200,1,127);
}

void weknows_init2()
{

}
void weknows_init1()
{
  weknowsshake(1,0);
}

void weknows_deinit()
{
//        free(face2);
}


layer weknows_main(int useframe)
{
        weknowsshake(0,useframe);
        return layer_make(vdscr,0,0,320,200,1,127);
}












/* rotating radial blur by craft / fudge */
static void initytabel()
{
        int t;
        for(t=-28;t<228;t++)
        {
                int y=t;
                if(t<0)y=0;
                if(t>198)y=198;
                if(t<2)y=2;
                if(t>196)y=196;

                zb_ytabel[t&255]=y*320*4;
        }
}

static void maketab(int v)
{
        int     t;

        for(t=0;t<256;t++)
        {
                zb_multab[t]=(t*v)>>8;
                zb_multab[t+256]=(t*(255-v))>>8;
        }
}

static void    upos(int x,float a, float c, int y, int *x1, int *x2)
{
        float p1;

        if(a*a<0.00000001)a=0.0001;
        p1=-c*y/a+x;


        if(a>0)
        {
                *x1=p1;
                *x2=320;
        }
        else
        {
                *x1=0;
                *x2=p1;
        }

        if(*x1<0)*x1=0;
        if(*x2>320)*x2=320;
}

static void    mix1(int *x1, int *x2, int *x3, int *x4)
{
        if(*x3>*x1)*x1=*x3;
        if(*x4<*x2)*x2=*x4;
}

static void    zblurkvad(int cx, int cy, float a, float b, float c, float d, int n)
{
        int y;
        float   dux,dvx,duy,dvy;
        int     m;

        dux=(a+1)*65536;
        dvx=(b)*65536;
        duy=(c)*65536;
        dvy=(d+1)*65536;

        switch(n&3)
        {
                case 0:
                {
                        zb_deltau=-dux;
                        zb_deltav=-dvx;

                        for(y=200-1;y>=0;y--)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,a,c,y-cy,&x1,&x2);
                                upos(cx,b,d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&vdscr[y*320+x1],&vdscr2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&vdscr2[y*320+x1],&vdscr[y*320+x1],m*4);else
                                zb_left(&vdscr[y*320+x2-1],vdscr,x2-x1,(x2-1-cx)*dux+(y-cy)*duy+cx*65536,(x2-1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 1:
                {
                        zb_deltau=dux;
                        zb_deltav=dvx;

                        for(y=200-1;y>=0;y--)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,-a,-c,y-cy,&x1,&x2);
                                upos(cx,b,d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&vdscr[y*320+x1],&vdscr2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&vdscr2[y*320+x1],&vdscr[y*320+x1],m*4);else
                                zb_right(&vdscr[y*320+x1],vdscr,x2-x1,(x1-cx)*dux+(y-cy)*duy+cx*65536,(x1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 3:
                {
                        zb_deltau=-dux;
                        zb_deltav=-dvx;

                        for(y=0;y<200;y++)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,a,c,y-cy,&x1,&x2);
                                upos(cx,-b,-d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&vdscr[y*320+x1],&vdscr2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&vdscr2[y*320+x1],&vdscr[y*320+x1],m*4);else
                                zb_left(&vdscr[y*320+x2-1],vdscr,x2-x1,(x2-1-cx)*dux+(y-cy)*duy+cx*65536,(x2-1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;
                case 2:
                {
                        zb_deltau=dux;
                        zb_deltav=dvx;

                        for(y=0;y<200;y++)
                        {
                                int     x1,x2,x3,x4;

                                upos(cx,-a,-c,y-cy,&x1,&x2);
                                upos(cx,-b,-d,y-cy,&x3,&x4);
                                mix1(&x1,&x2,&x3,&x4);
                                m=x2-x1;if(m<0)m=0;
                                if(n<0)memcpy(&vdscr[y*320+x1],&vdscr2[y*320+x1],m*4);else
                                if(n>=4)memcpy(&vdscr2[y*320+x1],&vdscr[y*320+x1],m*4);else
                                zb_right(&vdscr[y*320+x1],vdscr,x2-x1,(x1-cx)*dux+(y-cy)*duy+cx*65536,(x1-cx)*dvx+(y-cy)*dvy+cy*65536);
                        }
                }
                        break;

        }
}

static void box(int x1, int y1, int x2, int y2)
{
        int     x,y;

        for(y=y1;y<y2;y++)
        for(x=x1;x<x2;x++)
        {
              ((int*)vdscr)[y*320+x]=0;
        }
}

void    zblur(int cx, int cy, float a, float b, float c, float d)
{
/*        box(0,0,320,2);
        box(0,0,2,200);
        box(318,0,320,200);
        box(0,198,320,200);
  */
//        if(c>b)//(a+1)*(d+1)>b*c)
        if((a+1)>-(d+1))
        {
        zblurkvad(cx,cy,a,b,c,d,+4);
        zblurkvad(cx,cy,a,b,c,d,0);
        zblurkvad(cx,cy,a,b,c,d,1);
        zblurkvad(cx,cy,a,b,c,d,2);
        zblurkvad(cx,cy,a,b,c,d,3);
        zblurkvad(cx,cy,a,b,c,d,-4);
        zblurkvad(cx,cy,a,b,c,d,0);
        }
        else
        {
        zblurkvad(cx,cy,a,b,c,d,3+4);
        zblurkvad(cx,cy,a,b,c,d,3);
        zblurkvad(cx,cy,a,b,c,d,2);
        zblurkvad(cx,cy,a,b,c,d,1);
        zblurkvad(cx,cy,a,b,c,d,0);
        zblurkvad(cx,cy,a,b,c,d,3-4);
        zblurkvad(cx,cy,a,b,c,d,3);
        }

}





































void blob_init2()
{

}
void blob_init1()
{
  zoomblur_init();
  zoomblur_set(10);
  face2=(FacelistPL*)malloc(sizeof(FacelistPL)*20000);
  blob(1,0);


        initytabel();
}

void blob_deinit()
{
}


layer blob_main(int useframe)
{
        int r=255,g=255,b=255,fade;
        blob(0,useframe);
//        zoomblur(vdscr,160,100,0.98);


        if(sw[SW_BLOBRADIAL])
        {
                fmatrix m;
                static float a;
                a+=0.1;

                resetmatrixf(&m);
                rotatematrixzf(&m,0.009+0.008*sin(a));
                scalematrixf(&m,0.98+0.02*sin(0.5*a),0.975+0.025*sin(0.5*a),1);
                zblur(129+60*sin(a),100+60*cos(a*0.8),m.xu-1,m.xv,m.yu+0.01,m.yv-1);
        }





        fade=sw[SW_BLOBFADEVAL];

        return layer_make(vdscr,0,0,320,200,12,fade*256*256*256+b*65536+g*256+r);
}

/*
void main()
{ int test,i,vesaerror,vesamode,x,y;

  VBE_Init();
  vesaerror=Initvesamode();
  if (vesaerror==0) {
      wire();
//      kassescene();
    }
  VBE_Done();
  setmode(0x03);
}
*/

