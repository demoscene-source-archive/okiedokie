#ifndef __easy_to_use_vector_engine_h__
#define __easy_to_use_vector_engine_h__

#include <stdlib.h>

#include "convert.h"
#include "pcx.h"
#include "structs.h"
#include "salloc.h"
#include "engine.h"
#include "dloader.h"
#include "keyframe.h"


void        ee_init(char *pcxfile);
b_camera    *ee_loadscene(char *name3ds, char *namecam, char *nametexfile);
rgb1        *ee_animanddraw(b_camera *cam, float frames, rgb1 *screen);
void        ee_deinit(void);

#endif
