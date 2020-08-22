#ifndef __fade_h__
#define __fade_h__

static int fade_mixr;
static int fade_thecolor;
static char fade_multable[65536];

static void fade_init(void)
{
    int t;
    for(t=0;t<65536;t++)fade_multable[t]=((t>>8)*(t&255))>>8;
}

void fade(void *source, void *dest, int n, int val, int color);
#pragma aux fade =\
"        push    ebp                             ",\
"                                                ",\
"        shl     ebx,24                          ",\
"        shr     ebx,16                          ",\
"        xor     ebx,0xff00                      ",\
\
"        mov     ebp,edx                         ",\
"        shr     ebp,16                          ",\
"        and     ebp,255                         ",\
"        mov     al,[fade_multable+ebp+ebx]     ",\
"        shl     eax,16                          ",\
"        mov     ebp,edx                         ",\
"        shr     ebp,8                           ",\
"        and     ebp,255                         ",\
"        mov     ah,[fade_multable+ebp+ebx]     ",\
"        and     edx,255                         ",\
"        mov     al,[fade_multable+edx+ebx]     ",\
"        mov     [fade_mixr],eax                      ",\
\
"        xor     ebx,0xff00                      ",\
\
"        lea     esi,[esi+ecx*4]                 ",\
"        lea     edi,[edi+ecx*4]                 ",\
"        neg     ecx                             ",\
\
"        mov     dl,[esi+3+ecx*4]                ",\
"        mov     ebp,[esi+ecx*4]                 ",\
"@@loop12b:                                      ",\
"        mov     edx,ebp                         ",\
"        shr     edx,16                          ",\
"        and     edx,255                         ",\
"        mov     al,[fade_multable+edx+ebx]      ",\
"        mov     byte ptr [fade_thecolor+2],al            ",\
\
"        mov     edx,ebp                         ",\
"        shr     edx,8                           ",\
"        and     edx,255                         ",\
"        mov     al,[fade_multable+edx+ebx]     ",\
"        mov     byte ptr [fade_thecolor+1],al        ",\
"        and     ebp,255                         ",\
"        mov     al,[fade_multable+ebp+ebx]     ",\
"        mov     byte ptr [fade_thecolor],al          ",\
"        mov     ebp,[esi+ecx*4+4]               ",\
"        mov     eax,[fade_mixr]                      ",\
"        add     eax,[fade_thecolor]                  ",\
\
"        mov     [edi+ecx*4],eax                 ",\
"        inc     ecx                             ",\
"        jl      @@loop12b                       ",\
\
"        pop     ebp                             ",\
parm [esi] [edi] [ecx] [ebx] [edx]\
modify exact [esi edi eax ebx ecx edx];

#endif
