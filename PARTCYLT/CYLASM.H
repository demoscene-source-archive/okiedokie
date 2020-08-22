#ifndef __trace_ASM_h
#define __trace_ASM_h

typedef struct trace_p{
        char            b,g,r,uu1;
        short           u,v;
        char            t,uu2;
        unsigned short  c;
} trace_p;

#pragma aux trace_toscreen "_*" parm caller []
extern void trace_toscreen(trace_p *source, void *dest);
#pragma aux trace_uploadtexture "_*" parm caller []
extern void trace_uploadtexture(void *texture);
#pragma aux trace_calcgridrgb "_*" parm caller []
extern void trace_calcgridrgb(trace_p *screen);



#pragma aux trace_retrgb "_*" parm caller [esi] modify [eax ebx ecx edx esi edi ebp] value [eax]
extern int trace_retrgb(trace_p *screen);

extern float trace_biltabel[16*16*4];
extern char trace_gouraudtabel[512];
extern void *trace_texturetable[16];

#endif
