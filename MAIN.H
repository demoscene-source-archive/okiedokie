#ifndef __main_h
#define __main_h

#define scr ((char*)0xa0000)
#define N_SWITCHES 100

#include    <math.h>
#include    <stdlib.h>
#include    <conio.h>
#include    <stdio.h>

#include        "demolib\komms.h"
#include        "demolib\vesavbe.h"
#include        "demolib\col256.h"
#include        "demolib\convert.h"
#include        "demolib\layer.h"

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

void setsmode(unsigned short);
#pragma aux setsmode parm [ax] = "int 10h"

extern short   *VIDEO;         //Global video pointer
extern short   *usevideo;

extern rgb1    fake1[64000];

extern int grayscale;

extern  short   vidbuf[64000];

typedef enum
{
    videomode15,videomode24,videomode32
}main_videomode;

typedef enum
{
    videodata,videodata15,videodata24,videodata32
}main_videodatamode;

extern main_videomode main_thevideomode;

extern  int sw[N_SWITCHES];
extern  int framenum;

#endif
