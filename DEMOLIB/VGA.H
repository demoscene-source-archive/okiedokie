/*      VGA.H
 *
 * VGA hardware level routines for standard 320x200x256 mode
 *
 * Copyright 1995 Petteri Kangaslampi
*/

#ifndef __VGA_H
#define __VGA_H

#ifndef __MTYPES_H
typedef unsigned char uchar;
#endif

#ifdef __DJGPP__

#include <dos.h>

#define vgaWaitVR() { while ( !(inportb(0x03DA) & 8) ); }
#define vgaWaitNoVR() { while ( (inportb(0x03DA) & 8) ); }
#define vgaWaitDE() { while ( (inportb(0x03DA) & 1) ); }
#define vgaSetBorder(x) { inportb(0x03DA); outportb(0x03C0, 0x31); \
    outportb(0x03C0, x); }
						    


#else


#ifdef __386__

void vgaSetMode(int mode);
#pragma aux vgaSetMode = \
    "xor    ah,ah" \
    "int    10h" \
    parm [eax] \
    modify exact [eax];

void vgaClearScreen(uchar *buffer);
#pragma aux vgaClearScreen = \
    "mov    ecx,16000" \
    "cld" \
    "xor    eax,eax" \
    "rep    stosd" \
    parm [edi] \
    modify exact [eax ecx edi];

void vgaCopyScreen(uchar *buffer, uchar *dest);
#pragma aux vgaCopyScreen = \
    "mov    ecx,16000" \
    "cld" \
    "rep    movsd" \
    parm [esi] [edi] \
    modify exact [ecx esi edi];

void vgaPutPixel(uchar *buffer, int x, int y, int color);
#pragma aux vgaPutPixel = \
    "cmp    eax,319" \
    "ja     clip" \
    "cmp    ebx,199" \
    "ja     clip" \
    "imul   ebx,320" \
    "add    ebx,eax" \
    "mov    [edi+ebx],cl" \
    "clip:" \
    parm [edi] [eax] [ebx] [ecx] \
    modify exact [ebx];

void vgaWaitVR(void);
#pragma aux vgaWaitVR = \
    "mov    dx,03DAh" \
    "vr:" \
    "in     al,dx" \
    "test   al,8" \
    "jz     vr" \
    modify exact [eax edx];

void vgaWaitNoVR(void);
#pragma aux vgaWaitNoVR = \
    "mov    dx,03DAh" \
    "nvr:" \
    "in     al,dx" \
    "test   al,8" \
    "jnz    nvr" \
    modify exact [eax edx];

void vgaWaitDE(void);
#pragma aux vgaWaitDE = \
    "mov    dx,03DAh" \
    "de:" \
    "in     al,dx" \
    "test   al,1" \
    "jnz    de" \
    modify exact [eax edx];

void vgaSetBorder(int color);
#pragma aux vgaSetBorder = \
    "mov    dx,03DAh" \
    "in     al,dx" \
    "mov    dx,03C0h" \
    "mov    al,31h" \
    "out    dx,al" \
    "mov    al,bl" \
    "out    dx,al" \
    parm [ebx] \
    modify exact [eax edx];

void vgaSetRGB(int color, int r, int g, int b);
#pragma aux vgaSetRGB = \
    "mov    dx,03C8h" \
    "out    dx,al" \
    "inc    dx" \
    "mov    al,bl" \
    "out    dx,al" \
    "mov    al,cl" \
    "out    dx,al" \
    "mov    eax,esi" \
    "out    dx,al" \
    parm    [eax] [ebx] [ecx] [esi] \
    modify exact [eax edx];


#else

void vgaSetMode(int mode);
#pragma aux vgaSetMode = \
    "xor    ah,ah" \
    "int    10h" \
    parm [ax] \
    modify exact [ax];

void vgaWaitVR(void);
#pragma aux vgaWaitVR = \
    "mov    dx,03DAh" \
    "vr:" \
    "in     al,dx" \
    "test   al,8" \
    "jz     vr" \
    modify exact [ax dx];

void vgaWaitNoVR(void);
#pragma aux vgaWaitNoVR = \
    "mov    dx,03DAh" \
    "nvr:" \
    "in     al,dx" \
    "test   al,8" \
    "jnz    nvr" \
    modify exact [ax dx];

void vgaWaitDE(void);
#pragma aux vgaWaitDE = \
    "mov    dx,03DAh" \
    "de:" \
    "in     al,dx" \
    "test   al,1" \
    "jnz    de" \
    modify exact [ax dx];

void vgaSetBorder(int color);
#pragma aux vgaSetBorder = \
    "mov    dx,03DAh" \
    "in     al,dx" \
    "mov    dx,03C0h" \
    "mov    al,31h" \
    "out    dx,al" \
    "mov    al,bl" \
    "out    dx,al" \
    parm [bx] \
    modify exact [ax dx];

void vgaSetRGB(int color, int r, int g, int b);
#pragma aux vgaSetRGB = \
    "mov    dx,03C8h" \
    "out    dx,al" \
    "inc    dx" \
    "mov    al,bl" \
    "out    dx,al" \
    "mov    al,cl" \
    "out    dx,al" \
    "mov    ax,si" \
    "out    dx,al" \
    parm    [ax] [bx] [cx] [si] \
    modify exact [ax dx];
#endif

#endif

#endif
