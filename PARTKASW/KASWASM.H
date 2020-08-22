#ifndef __kaswasm_h
#define __kaswasm_h

typedef struct kasw_p{
        short   u,v;
        unsigned short   c;
} kasw_p;

#pragma aux kasw_toscreen "_*" parm caller []
extern void kasw_toscreen(kasw_p *source, void *dest);
#pragma aux kasw_toscreen2 "_*" parm caller []
extern void kasw_toscreen2(kasw_p *source, void *dest);
#pragma aux kasw_uploadtexture "_*" parm caller []
extern void kasw_uploadtexture(void *texture);
#pragma aux kasw_interpolatetexture "_*" parm caller []
extern void kasw_interpolatetexture(void *source, void *dest);

extern float kasw_biltabel[16*16*4];
extern char kasw_gouraudtabel[512];
extern rgb1 kasw_texture[256*256+257];

#endif
