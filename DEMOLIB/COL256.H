#ifndef __COL256_H
#define __COL256_H

#pragma aux to256col "_*" parm caller []
extern void to256col(void* src, void *dest);

#pragma aux rgb "_*" parm caller []
extern void rgb(int num, int r, int g, int b);

#endif
