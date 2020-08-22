/*
	PCX LOADER HEADER 0.1b
*/

#ifndef PCX_H
#define PCX_H

//	TYPEDEFS ------------------------------------------------

typedef struct pcx_header
{
	char	manufacturer;
	char	version;
	char	encoding;
	char	bitsperpixel;
	short	xmin,ymin,xmax,ymax;
	char	nplanes;
	short	bytesperline;
} pcx_header;

#define PCX_ADD2LINES (1<<30)

//  FUNCTION PROTOTYPES -------------------------------------

void	pcx_read_header(pcx_header *hdr, char *filename);
void	pcx_print_header(pcx_header *hdr);
void    pcx_read_data(pcx_header *hdr, char *filename, char *pic, int skip);
void*   pcx_read_picture(char *filename, int skip);
char*	pcx_read_palette(char *filename);

#endif
