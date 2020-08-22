#ifndef __BILERP_H
#define __BILERP_H

#pragma aux retinttab "_*" parm caller []
extern void retinttab(char **dest);

//#pragma aux bilinearinterpolation "_*" parm caller []
//extern void bilinearinterpolation(int *src, short *dest, char* texture);

//#pragma aux bilinearinterpolation2 "_*" parm caller []
//extern void bilinearinterpolation2(void *src, short *dest, char* texture);

#pragma aux bilinearinterpolation3 "_*" parm caller []
extern void bilinearinterpolation3(void *src, short *dest);

extern void submap3g(void *p1, void *p2, void *p3, int mapnr, char *dest);
#pragma aux submap3g "_*" parm caller []

extern void submap3gklip(void *p1, void *p2, void *p3, int mapnr, void *texture, char *dest);
#pragma aux submap3gklip "_*" parm caller []

extern void setscreenscale(float scalex, float scaley);
#pragma aux setscreenscale "_*" parm caller []

//extern void submap3(void *p1, void *p2, void *p3, int mapnr, char *dest);
//#pragma aux submap3 "_*" parm caller []

extern char* texturetable[256];
extern char gouraudtabel[512];

extern  float biltabel[1024];

typedef struct
{
	float	x,y,z;
	int	sx,sy;
	float	tx,ty,g;
} submap_pkt;

#endif
