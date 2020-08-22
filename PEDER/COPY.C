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

char baseheap[1024*1024*4];

#define byte unsigned char
#define word unsigned short
#define dword unsigned long
int ZMAXTEST=65536;
int SCREENX=320;
int SCREENY=200;
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

partikel psystem[17000];

#include "tinymath.h"
#include "tinyheap.h"
#include "int386.h"
#include "vesavbe.h"

#include "wire1.inc"

//screen variables
#define scr ((char*)0xa0000)
char *vesascr;
int lfb;      //linearframebuffer enabled
int screenbpp;  //screen bpp writemode
unsigned int vdscr[64000];
unsigned int tiletexture[65536];
unsigned int truetexture[65536];
char texture[65536];
char pal[65536];
dword flare[65536*2];
int pcol[10000];
float xp[10000];
float yp[10000];
int zp[10000];

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


#include "metagen.h"
#include "softt.h"

int checkvesamode(int xres,int yres, int bpp)
{ int i,vesaerror,vesamode;
  vesamode=VBE_FindMode (xres, yres, bpp);
  if (vesamode!=-1) {
    lfb=VBE_IsModeLinear(vesamode);
    if (lfb==0) VBE_SetMode(vesamode,0,1);
      else {
        VBE_SetMode(vesamode,1,1);
        vesascr=VBE_GetVideoPtr (vesamode);
      }
   return(0);
   }
   else {
    if (bpp==32) cputs("Error vesamode 320x200x32bpp not found.\n \r$");
    if (bpp==24) cputs("Error vesamode 320x200x24bpp not found.\n \r$");
    if (bpp==15) cputs("Error vesamode 320x200x15bpp not found.\n \r$");}
   return(-1);
}


int Initvesamode()
{ int i,vesaerror;
  screenbpp=24;
  vesaerror=checkvesamode(320,200,24);
  if (vesaerror==0) return(0);
  screenbpp=32;
  vesaerror=checkvesamode(320,200,32);
  if (vesaerror==0) return(0);
  screenbpp=15;
  vesaerror=checkvesamode(320,200,15);
  if (vesaerror==0) return(0);
  screenbpp=254;
  vesaerror=checkvesamode(640,480,32);
  if (vesaerror==0) return(0);
  screenbpp=255;
  vesaerror=checkvesamode(640,480,15);
  if (vesaerror==0) return(0);
  cputs("Sorry no available vesamodes try running Univbe 5.3 or newer...\n \r$");
  return(-1);
}


void memcpy32bto24b(void *, void *,int);
#pragma aux memcpy32bto24b =\
   "pusha",\
   "push ebp",\
   "xor eax,eax",\
   "shr ecx,5",\
   "xlloop: ",\
   "mov eax,[esi]",\
   "shl eax,8",\
   "mov ebp,[esi+4]",\
   "mov ebx,[esi+8]",\
   "shrd eax,ebp,8",\
   "mov [edi],eax",\
   "rol ebp,8",\
   "mov bp,bx",\
   "mov edx,[esi+12]",\
   "rol ebp,16",\
   "shr ebx,16",\
   "mov [edi+4],ebp",\
   "shl edx,8",\
   "mov dl,bl",\
   "mov [edi+8],edx",\
   "mov eax,[esi+16]",\
   "shl eax,8",\
   "mov ebp,[esi+20]",\
   "mov ebx,[esi+24]",\
   "shrd eax,ebp,8",\
   "mov [edi+12],eax",\
   "rol ebp,8",\
   "mov bp,bx",\
   "mov edx,[esi+28]",\
   "rol ebp,16",\
   "shr ebx,16",\
   "mov [edi+16],ebp",\
   "shl edx,8",\
   "mov dl,bl",\
   "mov [edi+20],edx",\
   "add edi,24",\
   "add esi,32",\
   "dec ecx",\
   "jnz xlloop",\
   "pop ebp",\
   "popa",\
parm [edi] [esi] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void memcpy32bhighres(void *, void *,int);
#pragma aux memcpy32bhighres =\
   "pusha",\
   "push ebp",\
   "mov dx,03d4h",\
   "mov ax,9",\
   "out dx,al",\
   "mov dx,03d5h",\
   "mov ax,1",\
   "out dx,al",\
   "xor eax,eax",\
   "shr ecx,2",\
   "xlloop: ",\
   "mov eax,[esi]",\
   "mov [edi],eax",\
   "mov [edi+4],eax",\
   "add edi,8",\
   "add esi,4",\
   "dec ecx",\
   "jnz xlloop",\
   "pop ebp",\
   "popa",\
parm [edi] [esi] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void memcpy32bto15b(void *, void *,int);
#pragma aux memcpy32bto15b =\
   "pusha",\
   "push ebp",\
   "xor eax,eax",\
   "shr ecx,2",\
   "xlloop: ",\
   "mov ebx,0",\
   "mov edx,[esi]",\
   "shl edx,16",\
   "shr edx,27",\
   "shl edx,5",\
   "mov ebx,[esi]",\
   "shl ebx,8",\
   "shr ebx,27",\
   "shl ebx,10",\
   "add edx,ebx",\
   "mov ebx,[esi]",\
   "shl ebx,24",\
   "shr ebx,27",\
   "add edx,ebx",\
   "mov [edi],dx",\
   "add edi,2",\
   "add esi,4",\
   "dec ecx",\
   "jnz xlloop",\
   "pop ebp",\
   "popa",\
parm [edi] [esi] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void memcpy32bto15bhighres(void *, void *,int);
#pragma aux memcpy32bto15bhighres =\
   "pusha",\
   "push ebp",\
   "mov dx,03d4h",\
   "mov ax,9",\
   "out dx,al",\
   "mov dx,03d5h",\
   "mov ax,0",\
   "out dx,al",\
   "xor eax,eax",\
   "shr ecx,2",\
   "xlloop: ",\
   "mov ebx,0",\
   "mov edx,[esi]",\
   "shl edx,16",\
   "shr edx,27",\
   "shl edx,5",\
   "mov ebx,[esi]",\
   "shl ebx,8",\
   "shr ebx,27",\
   "shl ebx,10",\
   "add edx,ebx",\
   "mov ebx,[esi]",\
   "shl ebx,24",\
   "shr ebx,27",\
   "add edx,ebx",\
   "mov [edi],dx",\
   "mov [edi+2],dx",\
   "add edi,4",\
   "add esi,4",\
   "dec ecx",\
   "jnz xlloop",\
   "pop ebp",\
   "popa",\
parm [edi] [esi] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void memcpy(void *, void *,int);
#pragma aux memcpy =\
   "pusha",\
   "mov ebx,ecx",\
   "shr ecx,2",\
   "shl ecx,2",\
   "sub ebx,ecx",\
   "shr ecx,2",\
   "rep movsd",\
   "mov ecx,ebx",\
   "rep movsb",\
   "popa",\
parm [edi] [esi] [ecx] \
modify exact [esi edi eax ebx ecx edx];

void memset(void *, int, int);
#pragma aux memset =\
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


void vesacopy(void *vscr)
{ int i;
    if (lfb!=0) {
        if (screenbpp==32) memcpy(vesascr,vscr,64000*4);
        if (screenbpp==24) memcpy32bto24b(vesascr,vscr,64000*4);
        if (screenbpp==15) memcpy32bto15b(vesascr,vscr,64000*4);
        if (screenbpp==254) memcpy32bhighres(vesascr+80*640,vscr,64000*4);
        if (screenbpp==255) memcpy32bto15bhighres(vesascr+140*640,vscr,64000*4);
    }
}

void setmode(char);
#pragma aux setmode =\
 "   pusha",\
 "   mov ah,0    ",\
 "   int 10h     ",\
 "   popa",\
parm [al] \
modify exact [esi edi eax ebx ecx edx] nomemory;


void vsinc();
#pragma aux vsinc =\
"        push    dx     ",\
"        PUSH    AX     ",\
"        mov     dx,3dah",\
"cwait:  in      al,dx  ",\
"        test    al,8   ",\
"        jne     cwait  ",\
"cwait1: in      al,dx  ",\
"        test    al,8   ",\
"        je      cwait1 ",\
"        POP     AX     ",\
"        pop     dx     ",\
modify exact [esi edi eax ebx ecx edx] nomemory;

void setborder(char);
#pragma aux setborder =\
 "   pusha            ",\
 "   mov bl,al        ",\
 "   mov dx,3c0h      ",\
 "   mov al,11h+32    ",\
 "   out dx,al        ",\
 "   mov al,bl        ",\
 "   out dx,al        ",\
 "   popa             ",\
parm [al] \
modify exact [esi edi eax ebx ecx edx] nomemory;

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))


static byte wu_table_r[256];
static byte wu_table_g[256];
static byte wu_table_b[256];

static int _xinc, _yinc, _slope;


void set_wu_color(float r, float g, float b) {  // 0..1
    int i;
    for(i=0; i<256; i++)
        wu_table_r[i] = min(r*i,255);
    for(i=0; i<256; i++)
        wu_table_g[i] = min(g*i,255);
    for(i=0; i<256; i++)
        wu_table_b[i] = min(b*i,255);
}


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

    int xsize = SCREENX;
    int ysize = SCREENY;

    int p1=VISIBLE, p2=VISIBLE;
    if(x1<1)                p1 |= LLEFT;
    if(x1>SCREENX-2)   p1 |= RRIGHT;
    if(x2<1)                p2 |= LLEFT;
    if(x2>SCREENX-2)   p2 |= RRIGHT;
    if((p1&LLEFT) && (p2&LLEFT)) return;
    if((p1&RRIGHT) && (p2&RRIGHT)) return;

    if((p1&(LLEFT|RRIGHT)) || (p2&(LLEFT|RRIGHT))) {
        a = (double)(y2-y1)/(x2-x1);
        k = y1-a*x1;
        x1 = min(max(x1,1),SCREENX-2);
        y1 = a*x1+k;
        x2 = min(max(x2,1),SCREENX-2);
        y2 = a*x2+k;
    }

    if(y1<1)                 p1 |= TOP;
    if(y1>SCREENY-2)    p1 |= BOTTOM;
    if(y2<1)                 p2 |= TOP;
    if(y2>SCREENY-2)    p2 |= BOTTOM;
    if((p1&TOP) && (p2&TOP)) return;
    if((p1&BOTTOM) && (p2&BOTTOM)) return;

    if((p1&(TOP|BOTTOM)) || (p2&(TOP|BOTTOM))) {
        a = (double)(x2-x1)/(y2-y1);
        k = x1-a*y1;
        y1 = min(max(y1,1),SCREENY-2);
        x1 = min(max(a*y1+k,1),SCREENX-2);
        y2 = min(max(y2,1),SCREENY-2);
        x2 = min(max(a*y2+k,1),SCREENX-2);
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
    deltax = ffabs(deltax);
    if(deltay > 0) {
        yinc = 4*SCREENX;
    } else {
        yinc = -4*SCREENX;
        y1 = -y1;
        y2 = -y2;
    }
    deltay = ffabs(deltay);
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

    prestep = ffmod(y1cor,1);
    prestep *= 4294967296.0;

 //   byte *dest = (byte*)screen + y1i*yinc + x1i*xinc;
//    byte *dest = (byte*)screen + y1i*yinc + x1i*xinc;

    _xinc = xinc;
    _yinc = yinc;
    _slope = slopei;
    asm_wuline32addclip(vdscr+ (y1i*yinc + x1i*xinc)/4,prestep,deltaxi);
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

void kassescene()
{   int sub2,sub,temp,test,i,j,a,b,col,zbufcount,c,u,v,dst,testinc,boxsize;
    word zbuf;
    float x,y,z,ppx,ppy,ppz,phiz,phiy,phix,tanfov,splength,radius,scale;
    float kuglecx,kuglecy,kuglecz;
    int xpm, ypm, zpm, dup,nl;

   for (a=0;a<256;a++)
   for (b=0;b<256;b++)
   texture[a*256+b]=fexp(((181-fsqrt((a-128)*(a-128)+(b-128)*(b-128)))*1.4)/46.0);

   for (a=0;a<256;a++)
   for (b=0;b<256;b++)
   texture[a*256+b]=(((181-fsqrt((a-128)*(a-128)+(b-128)*(b-128)))*1.4)+texture[a*256+b])/2;

       for(v=0; v<256; v++) {
           for(u=0; u<256; u++) {
               dst = ((u<<8) & 0xf800)+(u & 0x0007)+((v<<3) & 0x07f8);
               tiletexture[dst] = texture[u+v*256];
              }
          }
    boxsize=20;
    scale=15;
    numpoints=0;
    numfaces=0;
    generategridplane(0,0,1,numpoints,numfaces,boxsize,1,scale);
    generategridplane(0,0,-1,numpoints,numfaces,boxsize,2,scale);
    generategridplane(0,-1,0,numpoints,numfaces,boxsize,1,scale);
    generategridplane(0,1,0,numpoints,numfaces,boxsize,2,scale);
    generategridplane(-1,0,0,numpoints,numfaces,boxsize,1,scale);
    generategridplane(1,0,0,numpoints,numfaces,boxsize,2,scale);
    kuglecx=0;
    kuglecy=0;
    kuglecz=0;
    radius=150;
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
    tanfov = 1.20;
    sizex=tanfov*SCREENX / 2 / tanfov;
    sizey=tanfov*1.2*SCREENY / 2 / 1.2/ tanfov;

    scalex=tanfov;
    scaley=tanfov*1.2*1.18;

    camera1_3ds.position.x=-190;
    camera1_3ds.position.y=2;
    camera1_3ds.position.z=1000;
    camera1_3ds.target.x=0;
    camera1_3ds.target.y=0;
    camera1_3ds.target.z=0;
    camera1_3ds.roll=-3.1415/2;
    test=0;
    framenum=0;
    while ((!kbhit()) &&(test<3000))
    {
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
    kuglecx=fsin(phix)*80;
    kuglecy=fsin(phiy)*80;
    kuglecz=fsin(phiz)*80;
    radius=140;
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
    kuglecx=fsin(phix*1.7)*80;
    kuglecy=fsin(phiy*1.3)*80;
    kuglecz=fsin(phiz*1.2)*80;
    radius=140;
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
    kuglecx=fsin(phix*1.9)*80;
    kuglecy=fsin(phiy*1.7)*80;
    kuglecz=fsin(phiz*1.3)*80;
    radius=140;
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
     vesacopy(vdscr);
     memset(vdscr,0,64000*4);

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
//      putmob(160,100,framenum*10,(dword*)flare);
      phix+=0.01757;
      phiy+=0.01538;
      phiz+=0.00224;
/*       setborder(0);
       vsinc();
       setborder(230);*/
       if (testinc==1) test++;
       if (testinc==0) test--;
       if (test<1) testinc=1;
       if (test>30) testinc=0;
       test=0;
       pos+=1;
       framenum++;
    }
}

float sintab[1024];
int pointsbal[8000*3];

void wire()
{   int slength,xpp,ypp,du,dv,u,v,fadecol,ddl,ddl2,lll,lll2,ll,dl,dest,lxx,lyy,lxx2,lyy2,xx2,yy2,ddx2,ddy2,ddx,ddy,dx,dy,xx,yy,action,zpos,pos1,a,b,zadd,rndx,rndy,rndz,zheight,effectnum,xr,yr,test;
    float vdu,vdv,c1,c2,c3,c4,zhorizon,invdu,invdv,w1,w2,w3,w4;
    float phix,phiy,phiz,x1,y1,x2,y2,x3,y3,x,y,z;
    float planz,dd,ppx,ppy,ppz,size,randfunc2;
    float dcol,z1;
    int rnd,pnum,col,pfade,i,randfunc,xpos,ypos;

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

    while ((!kbhit()) &&(test<3000)) {
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
            z = (ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2])-zpos;
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
           wuline32addclip(x1,y1,x3,y3,vdscr);
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
    if (dcol<0.6) dcol+=0.0005;
    vesacopy(vdscr);
    memset(vdscr,70,64000*4);
    if (zpos>900) zpos-=6;
    phix+=0.0075;
    phiy+=0.0083;
    phiz+=0.0093;
    }
}




void main()
{ int test,i,vesaerror,vesamode,x,y;
  hinit (baseheap, 1024*1024*4);

  face2=(FacelistPL*)halloc(sizeof(FacelistPL)*20000);
  VBE_Init();
  vesaerror=Initvesamode();
  if (vesaerror==0) {
      wire();
//      kassescene();
    }
  VBE_Done();
  setmode(0x03);
}

