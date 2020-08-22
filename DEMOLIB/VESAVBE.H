#ifndef _VESAVBE_H
#define _VESAVBE_H

 /*-----------------05-14-97 05:19pm-----------------
  *
  *	SUBMiSSiVES VESA VBE 2.0 Application Core
  *	-----------------------------------------
  *
  * tested and works well under Dos4G, PmodeW and Win95
  *
  * This code also works for buggy VBE's and finds
  * the mode even if the modelist is placed in dos-memory.
  *
  * --------------------------------------------------*/

 /*-----------------05-25-97 05:44pm-----------------
  *	 Diagnostic and Configuration Switches
  * --------------------------------------------------*/

#define DISABLE_PM_EXTENSIONS
// #define DISABLE_LFB

#include <i86.h>
#include <string.h>

#pragma pack (1);

#ifdef __cplusplus
extern "C" {
#endif

typedef void ( * tagSetDisplayStartType )(short x, short y);
typedef void ( * tagSetBankType )(short bnk);

typedef struct
{
  unsigned short int segment;
  unsigned short int selector;
} DPMI_PTR;

typedef struct {
  char	vbeSignature[4];
  unsigned short vbeVersion;
  char *   OemStringPtr;
  unsigned long  Capabilities;
  unsigned short * VideoModePtr;
  unsigned short TotalMemory;
  unsigned short OemSoftwareRev;
  char *   OemVendorNamePtr;
  char *   OemProductNamePtr;
  char *   OemProductRevPtr;
  char	   Reserved[222];
  char	   OemData[256];
} VBE_VbeInfoBlock ;

typedef struct {
  unsigned short ModeAttributes;
  char WinAAttributes;
  char WinBAttributes;
  unsigned short Granularity;
  unsigned short WinSize;
  unsigned short WinASegment;
  unsigned short WinBSegment;
  void *	 WinFuncPtr;
  unsigned short BytesPerScanline;
  unsigned short XResolution;
  unsigned short YResolution;
  char XCharSize;
  char YCharSize;
  char NumberOfPlanes;
  char BitsPerPixel;
  char NumberOfBanks;
  char MemoryModel;
  char BankSize;
  char NumberOfImagePages;
  char Reserved;
  char RedMaskSize;
  char RedFieldPosition;
  char GreenMaskSize;
  char GreenFieldPosition;
  char BlueMaskSize;
  char BlueFieldPosition;
  char RsvdMaskSize;
  char RsvdFieldPosition;
  char DirectColorModeInfo;
  void * PhysBasePtr;
  void * OffScreenMemOffset;
  unsigned short OffScreenMemSize;
  char	reserved2[206];
} VBE_ModeInfoBlock;

// ------------------------------------------------------------------------
//			    Setup Routines
// ------------------------------------------------------------------------

void VBE_Init (void);

  /*
   *
   * Must be called before any of the other functions will be called,
   * otherwise your system will crash
   *
   */

void VBE_Done (void);

  /*
   *
   * Frees all memory allocated by VBE_Init.. Not a must to call, but
   * it frees about 1k
   *
   */


// ------------------------------------------------------------------------
//			    DPMI Support Functions..
// ------------------------------------------------------------------------


void DPMI_AllocDOSMem (short int paras, DPMI_PTR *p);

 /*
  *
  * Allocate paras numbers of memory-paragraphs (16 bytes)
  * in realmode memory and store the segment/selector in
  * structure p
  *
  */

void DPMI_FreeDOSMem (DPMI_PTR *p);

 /*
  *
  * Free a allocated ralmode memory block
  *
  */

void * DPMI_MAP_PHYSICAL (void *p, long size);

 /*
  *
  * Create a physical mapping for the memory at p to p+size.
  * Returns that pointer (doesn't work with realmode memory)
  *
  */

// ------------------------------------------------------------------------
//			VBE/VESA Low Level Functions
// ------------------------------------------------------------------------

int VBE_Test (void);

 /*
  *
  * Returns true if a VBE BIOS-extension version 2.0 or greater is available
  * Returns zero otherwise
  *
  */

void VBE_Controller_Information  (VBE_VbeInfoBlock __far * a);

 /*
  *
  * Request the VberInfoBlock. Look into the VBE 2.0 Specs. for more
  * information.
  *
  */


int  VBE_IsModeLinear (short Mode);

 /*
  *
  * Returns nonzero if specified mode supports a linear framebuffer
  *
  */

extern tagSetBankType VBE_SetBank;

 /*
  * void VBE_SetBank (short bnk);
  *
  * Sets the A-Window to the specified bank. 0xa0000 points to another
  * location of the video memory. Be aware, that the bank is dependent
  * of the granularity field in the modeinfoblock.
  *
  */

unsigned int VBE_VideoMemory (void);

 /*
  *
  * Return the amount of display memory available  (in bytes)
  *
  */

void VBE_Mode_Information (short Mode, VBE_ModeInfoBlock __far * a);

 /*
  *
  * Request the ModeInfoBlock for the specified mode. Look into the VBE 2.0
  * specs. for more information.
  *
  */

int VBE_FindMode (int xres, int yres, char bpp);

 /*
  *
  * Search the modelist for the specified mode. bpp is BytesPerPixel
  * (8 for 256 colors). Returns -1 on error
  *
  */

void VBE_SetMode (short Mode, int linear, int clear);

 /*
  *
  * Set VGA to the specified VBE-Mode. Use linear frame buffer if linear==1
  * Clear the video-memory if clear==1
  *
  */

char * VBE_GetVideoPtr (int mode);

 /*
  * Get the (mapped) pointer to the linear frame buffer for the specified
  * mode. May fail on weired DPMI-Systems (tested: pmodew, dos4g, win95)
  */

extern tagSetDisplayStartType VBE_SetDisplayStart;

 /* void VBE_SetDisplayStart (short x, short y); */

 /*
  * Adjust the position of the logical upper left pixel.
  * (for scrolling & page flipping)
  */

void VBE_SetPixelsPerScanline (short Pixels);
 /*
  * Sets the number of pixels per scanline. Warning, not all VESA VBE
  * implementations support this feature. However, it might be usefull
  * if you have to deal with non linear frame buffers.
  */

short VBE_MaxBytesPerScanline (void);
 /*
  * Returns the maxumum bytes per scanline. Unfortunately this function is
  * more or less useless because most vesa implementations doesn't give valid
  * results.
  */


void VBE_SetDACWidth (char bits);

 /*
  * Sets the DAC into the 6 (default) or 8 bit color mode. You have to
  * check first if the DAC supports 8 bit using the VBE_8BitDAC function.
  *
  */

int VBE_8BitDAC (void);

 /*
  * Returns non-zero, if the DAC can be switched into the 8-Bit mode.
  * (only usefull in 256 color modes. Gives you a better color-control)
  *
  */

#ifdef __cplusplus
}
#endif

#endif
