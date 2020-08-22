#ifndef __script_h_
#define __script_h_

typedef struct s_script{
        int     pos;
        int     row;
        int     swi;
        int     val;
}s_script;

s_script  *s_load(char *filename, int scriptsize);

#endif
