#ifndef __file_h__
#define __file_h__

typedef struct FFILE{
        int     virtual;
        FILE    *file;
        int     pos,length;
} FFILE;

void    lnk_dir(void);
int     ffeof(FFILE *f);
void    ffclose(FFILE *f);
FFILE *ffopen(char *name, char *mode);
int     ffgetc(FFILE *f);
int     fungetc(int c, FFILE *f);
void    lnk_deinit(void);
lnk_init(char *name, int i);

int     ffgetbyte(FFILE *a);
int     ffgetword(FFILE *a);
int     ffgetsword(FFILE *a);
int     ffgetbigword(FFILE *a);
int     ffgetbigsword(FFILE *a);
int     ffgetdword(FFILE *a);
int     ffgetbigdword(FFILE *a);
void    ffskip(int j,FFILE *f);

#endif
