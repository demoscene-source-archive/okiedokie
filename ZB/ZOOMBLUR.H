/**************************************************
*RadialZoomblur by Craft/fudGe (november 1998)    *
*zoomblur_init() initialisering                   *
*zoomblur_set()  (om)definer blandingsforholdet   *
*zoomblur()      blur                             *
**************************************************/

#ifndef __zoomblur_h
#define __zoomblur_h

void    zoomblur_init(void);
void    zoomblur_set(int v);
void    zoomblur(int *screen, int x, int y, float zoom);

#endif
