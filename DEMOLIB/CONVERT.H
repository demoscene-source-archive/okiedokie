#ifndef convert_
#define convert_

typedef struct rgb1{
//    char    r,g,b,o;
    char    b,g,r,o;
}rgb1;


#pragma aux con24to15 "_*" parm caller []
extern void con24to15(rgb1 *src, short *dest);


#pragma aux con15t15 "_*" parm caller []
extern void con15t15(void *src, void *dest);
#pragma aux con15t24 "_*" parm caller []
extern void con15t24(void *src, void *dest);
#pragma aux con15t32 "_*" parm caller []
extern void con15t32(void *src, void *dest);

#pragma aux con24t15 "_*" parm caller []
extern void con24t15(void *src, void *dest);
#pragma aux con24t24 "_*" parm caller []
extern void con24t24(void *src, void *dest);
#pragma aux con24t32 "_*" parm caller []
extern void con24t32(void *src, void *dest);

#pragma aux con32t15 "_*" parm caller []
extern void con32t15(void *src, void *dest);
#pragma aux con32t24 "_*" parm caller []
extern void con32t24(void *src, void *dest);
#pragma aux con32t32 "_*" parm caller []
extern void con32t32(void *src, void *dest);

#pragma aux con15 "_*" parm caller []
extern void con15(void *src, void *dest, int n);
#pragma aux con24 "_*" parm caller []
extern void con24(void *src, void *dest, int n);
#pragma aux con32 "_*" parm caller []
extern void con32(void *src, void *dest, int n);

#endif
