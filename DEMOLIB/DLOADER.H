#ifndef dloader
#define dloader
#include "file3ds.h"
#include "filer.h"
#include <assert.h>
#include "engine.h"

void    dloader_settexturehandler(b_scene *sce, void *texture);
void    dloader_settexturehandlerpcx(b_scene *sce, char *texname, int ttype);
b_object *loadobject(char *filename, char *objectname);

#endif
