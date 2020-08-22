/*
	TEXTURE HANDLER HEADER 0.1b
*/

#ifndef TH_H
#define TH_H

#include    "structs.h"

//	TYPEDEFS ---------------------------------------------------

//	moved to structs.h

//	FUNCTION PROTOTYPES ----------------------------------------

void    th_transformtexture(char *tex, int n);
void	th_inittexturehandler(th_texturehandler *handler, char *filename);
int	th_gettexturenumber(th_texturehandler *handler,char *name);
void*	th_gettexturepointer(th_texturehandler *handler, char *name);
void*   th_gettextureslot(th_texturehandler *handler, char *name);

#endif
