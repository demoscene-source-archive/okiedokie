/* by craft / fudge */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//#include "scan.h"

#include "file.h"

/*
typedef struct FFILE{
        int     virtual;
        FILE    *file;
        int     pos,length;
} FFILE;
*/

typedef struct filel{
        char    *name;
        char    *mode;
        int     offset;
        int     length;
        struct filel     *next;
} filel;


int     lnk_initialized=0;
filel   *lnk_files=NULL;
char    *lnk_datafilename;

static void lnk_startposition(FILE *f)
{
        int n;
        fseek(f,-sizeof(int),SEEK_END);
        lnk_readstruct(f,&n,sizeof(int));
        fseek(f,-n-sizeof(int),SEEK_END);
}

void    lnk_dir2(filel *p)
{
        if(p)
        {
                printf("%s %s %i %i\n",p->name,p->mode,p->offset,p->length);

                lnk_dir2(p->next);
        }

}

void    lnk_dir()
{
        lnk_dir2(lnk_files);
}


filel*  lnk_findfile(filel* files, char *name, char *mode)
{
        if(files)
        {
                if(!strcmp(name,files->name))
                {
                        if(strcmp(mode,files->mode))
                        {
                                printf("File-linker-error. Wrong mode :%s,%s\n",name,mode);
                                exit(1);
                        }
                        return files;
                }
                else return lnk_findfile(files->next,name,mode);
        }
        return NULL;
}

filel   *lnk_newfilel(char *name, char *mode, filel *next)
{
        filel   *f;

        assert(f=malloc(sizeof(filel)));
        assert(f->name=malloc(strlen(name)+1));
        assert(f->mode=malloc(strlen(mode)+1));

        strcpy(f->name,name);
        strcpy(f->mode,mode);

        f->next=next;

        return f;
}

filel   *lnk_addfile(char *name, char *mode)
{
        if(lnk_findfile(lnk_files,name,mode))
        {
               return lnk_findfile(lnk_files,name,mode);
        }
        else
        return lnk_files=lnk_newfilel(name,mode,lnk_files);
}

filel   *lnk_addfile2(char *mode, char *name)
{
        return lnk_addfile(name,mode);
}

int     ffeof(FFILE *f)
{
        if(f->virtual) return (f->pos>f->length);
        return feof(f->file);
}

void    ffclose(FFILE *f)
{
        if(f)
        {
                if(f->file)fclose(f->file);
                free(f);
        }
}

FFILE *ffopen(char *name, char *mode)
{
        FFILE *file;


        if(lnk_initialized)
        {
                filel   *filedata;

                if(filedata=lnk_findfile(lnk_files,name,mode))
                {
                        assert(file=malloc(sizeof(FFILE)));

                        assert(file->file=fopen(lnk_datafilename,"rb"));
                        lnk_startposition(file->file);
                        file->virtual=1;
                        file->pos=0;
                        file->length=filedata->length;

                        assert(!fseek(file->file,filedata->offset,SEEK_CUR));

                        return file;
                }
        }
        {
                FILE *f;

                lnk_addfile(name,mode);
                f=fopen(name,mode);
                if(f==NULL)return NULL;

                assert(file=malloc(sizeof(FFILE)));

                file->virtual=0;
                file->file=f;
        }

        return file;
}

int     ffgetc(FFILE *f)
{
        if(f->virtual)
        {
                f->pos++;
                if(f->pos<=f->length)
                {
                        return fgetc(f->file); //evt. getc
                }
                else return EOF;
        }
        else
        {
                return fgetc(f->file);
        }
}

int     fungetc(int c, FFILE *f)
{
        if(f->virtual)
        {
                f->pos--;
                if(f->pos<=f->length)
                {
                        return ungetc(c,f->file);
                }
                else return EOF;
        }
        else
        {
                return ungetc(c,f->file);
        }
}

void    lnk_writestring(FILE *f, char *s)
{
        do
        {
                fputc(*s,f);
                s++;
        }
        while(*s);
        fputc(*s,f);
}

char    *lnk_readstring(FILE *f)
{
        static char temp[1024];
        char    *s=temp;

        *s=fgetc(f);
        while(*s)
        {
                s++;
                *s=fgetc(f);
        }

        s=malloc(strlen(temp)+1);
        strcpy(s,temp);

        return s;
}

void    lnk_writestruct(FILE *f, void *p, int size)
{
        fwrite(p,size,1,f);
}

void    lnk_readstruct(FILE *f, void *p, int size)
{
        fread(p,size,1,f);
}

int     lnk_filelength(char *name, char *mode)
{
        int     c,l=0;
        FILE    *f;

        assert(f=fopen(name,mode));

        while((c=fgetc(f))!=EOF)
        {
                l++;
        }

        fclose(f);

        return l;
}

void    lnk_append(FILE *f,char *name, char *mode)
{
        int     c;
        FILE    *f2;

        assert(f2=fopen(name,mode));

        while((c=fgetc(f2))!=EOF)
        {
                fputc(c,f);
        }

        fclose(f2);
}

void    lnk_deinit(void)
{
        if(lnk_initialized==0) //Write the data file
        {
                FILE    *datafile;
                filel   *f=lnk_files;
                int     offset;
                int     n=0;

                assert(datafile=fopen(lnk_datafilename,"wb"));

                while(f)
                {
                        f->length=lnk_filelength(f->name,f->mode);

                        f=f->next;
                        n++;
                }
                lnk_writestruct(datafile,&n,sizeof(int));
                f=lnk_files;
                while(f)
                {
                        lnk_writestruct(datafile,f,sizeof(filel));
                        lnk_writestring(datafile,f->name);
                        lnk_writestring(datafile,f->mode);

                        f=f->next;
                }
                offset=ftell(datafile);
                rewind(datafile);
                lnk_writestruct(datafile,&n,sizeof(int));
                f=lnk_files;
                while(f)
                {
                        f->offset=offset;
                        offset+=f->length;
                        lnk_writestruct(datafile,f,sizeof(filel));
                        lnk_writestring(datafile,f->name);
                        lnk_writestring(datafile,f->mode);

                        f=f->next;
                }
                f=lnk_files;
                while(f)
                {
                        lnk_append(datafile,f->name,f->mode);

                        f=f->next;
                }

                {       //write datafilelength at end of file
                        int i;

                        i=ftell(datafile);
                        lnk_writestruct(datafile,&i,sizeof(int));
                }

                fclose(datafile);
        }
}

lnk_init(char *name, int i) //i=0 read original files, and write datafile on exit.
                            //i=1 read the datafile
{
        lnk_datafilename=name;

        lnk_initialized=i;

        if(lnk_initialized)
        {
                FILE*   f;
                int     n,t;

                assert(f=fopen(name,"rb"));
                lnk_startposition(f);

                lnk_readstruct(f,&n,sizeof(int));
                for(t=0;t<n;t++)
                {
                        filel   a,*b;

                        lnk_readstruct(f,&a,sizeof(filel));
                        b=lnk_addfile2(lnk_readstring(f),lnk_readstring(f));
                        b->offset=a.offset;
                        b->length=a.length;
                }

                fclose(f);
        }
}




int     ffgetbyte(FFILE *a)
{
        return ((unsigned char)ffgetc(a));
}

int     ffgetword(FFILE *a)
{
        char    b[4];
        b[0]=ffgetbyte(a);
        b[1]=ffgetbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     ffgetsword(FFILE *a)
{
        char    b[4];
        b[0]=ffgetbyte(a);
        b[1]=ffgetbyte(a);
        return ((signed short*)b)[0];
}

int     ffgetbigword(FFILE *a)
{
        char    b[4];
        b[1]=ffgetbyte(a);
        b[0]=ffgetbyte(a);
        b[2]=0;
        b[3]=0;
        return ((int*)b)[0];
}

int     ffgetbigsword(FFILE *a)
{
        char    b[4];
        b[1]=ffgetbyte(a);
        b[0]=ffgetbyte(a);
        return ((signed short*)b)[0];
}


int     ffgetdword(FFILE *a)
{
        char    b[4];
        b[0]=ffgetbyte(a);
        b[1]=ffgetbyte(a);
        b[2]=ffgetbyte(a);
        b[3]=ffgetbyte(a);
        return ((int*)b)[0];
}

int     ffgetbigdword(FFILE *a)
{
        char    b[4];
        b[3]=ffgetbyte(a);
        b[2]=ffgetbyte(a);
        b[1]=ffgetbyte(a);
        b[0]=ffgetbyte(a);
        return ((int*)b)[0];
}

void    ffskip(int j,FFILE *f)
{
        for(;j>0;j--){ffgetc(f);};
}








/*
void    main()
{
        FFILE *f;

        lnk_init("DATA.DAT",1);
        lnk_dir(lnk_files);

        assert(f=ffopen("file.c","r"));

        {
                int     i;
                while((i=ffgetc(f))!=EOF)
                {
                        printf("%c",i);
                }
                printf("\n");
        }

        ffclose(f);

        lnk_deinit();
}
*/

