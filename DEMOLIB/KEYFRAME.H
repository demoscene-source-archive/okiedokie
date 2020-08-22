#ifndef keyframe_h_
#define keyframe_h_


#include "structs.h"
#include "file3ds.h"
#include "filer.h"
#include "engine.h"
#include "spline.h"
#include "assert.h"

void    keyframe_loadcameraanim(char *filename, char *cameraname, b_camera* cam);
void    keyframe_animcam(b_camera* cam,float time);
void    keyframe_loadanim(char *filename, b_object* root);
void    keyframe_animobjects(b_object* obj,float time);

#endif
