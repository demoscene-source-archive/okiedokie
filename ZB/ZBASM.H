#ifndef __ZASM_h
#define __ZASM_h


extern char zb_multab[256*2];
extern int zb_deltau,zb_deltav;
extern float zb_mix;
extern float zb_biltab[1024];
extern int zb_ytabel[256];
#pragma aux zb_ytabel "_*"

#pragma aux zb_right "_*" parm caller []
extern void zb_right(void *start, void *screen, int n, int u, int v);
#pragma aux zb_left "_*" parm caller []
extern void zb_left(void *start, void *screen, int n, int u, int v);

#pragma aux zb_rightnoclip "_*" parm caller []
extern void zb_rightnoclip(void *start, void *screen, int n, int u, int v);
#pragma aux zb_leftnoclip "_*" parm caller []
extern void zb_leftnoclip(void *start, void *screen, int n, int u, int v);

#endif
