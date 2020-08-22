#include "VESAVBE.H"
#include <stdio.h>
#include <malloc.h>
#include <conio.h>

 /*
  * Structure to do the Realmode Interrupt Calls.
  */

static struct rminfo {
  unsigned long EDI;
  unsigned long ESI;
  unsigned long EBP;
  unsigned long reserved_by_system;
  unsigned long EBX;
  unsigned long EDX;
  unsigned long ECX;
  unsigned long EAX;
  unsigned short flags;
  unsigned short ES,DS,FS,GS,IP,CS,SP,SS;
} RMI;

 /*
  * Structures that hold the preallocated DOS-Memory Aeras
  */

static DPMI_PTR VbeInfoPool = {0,0};
static DPMI_PTR VbeModePool = {0,0};

/*
 * Structures, that hold the Informations needed to invoke
 * PM-Interrupts
 */

static union  REGS regs;
static struct SREGS sregs;

 /*
  * Some informations 'bout the last mode which was set
  * These informations are required to compensate some differencies
  * between the normal and direct PM calling methods
  */

static short  vbelastbank = -1;
static unsigned long BytesPerScanline;
static unsigned char BitsPerPixel;

 /*
  * Protected Mode Direct Call Informations
  */

void VBE_InitPM (void);
static void * pm_setwindowcall;
static void * pm_setdisplaystartcall;
static void    *pmcode = NULL;

/*
 *  This function pointers will be initialized after you called
 *  VBE_Init. It'll be set to the realmode or protected mode call
 *  code
 */

tagSetDisplayStartType VBE_SetDisplayStart;
tagSetBankType         VBE_SetBank;

static void PrepareInfoBlock (VBE_VbeInfoBlock *VBEInfo)
{
  memset (VBEInfo, 0, sizeof (VBEInfo));
  *((unsigned long*) &((*VBEInfo).vbeSignature)) = 0x56423532;
}

static void PrepareRegisters (void)
{
  memset(&RMI,0,sizeof(RMI));
  memset(&sregs,0,sizeof(sregs));
  memset(&regs,0,sizeof(regs));
}

static void RMIRQ (char irq)
{
  regs.w.ax = 0x0300;               // Simulate Real-Mode interrupt
  regs.h.bl = irq;
  regs.h.bh = 0;
  regs.w.cx = 0;
  sregs.es = FP_SEG(&RMI);
  regs.x.edi = FP_OFF(&RMI);
  int386x( 0x31, &regs, &regs, &sregs);
}

void DPMI_AllocDOSMem (short int paras, DPMI_PTR *p)
{
  /* DPMI call 100h allocates DOS memory */
  PrepareRegisters();
  regs.w.ax=0x0100;
  regs.w.bx=paras;
  int386x( 0x31, &regs, &regs, &sregs);
  p->segment=regs.w.ax;
  p->selector=regs.w.dx;
}

void DPMI_FreeDOSMem (DPMI_PTR *p)
{
  /* DPMI call 101h free DOS memory */
  memset(&sregs,0,sizeof(sregs));
  regs.w.ax=0x0101;
  regs.w.dx=p->selector;
  int386x( 0x31, &regs, &regs, &sregs);
}

void * DPMI_MAP_PHYSICAL (void *p, long size)
{
  /* DPMI call 101h free DOS memory */
  PrepareRegisters();
  regs.w.ax=0x0800;
  regs.w.bx=((long)p)>>16;
  regs.w.cx=((long)p)&0xffff;
  regs.w.si=size>>16;
  regs.w.di=size&0xffff;
  int386x( 0x31, &regs, &regs, &sregs);
  return ((void *) ((regs.w.bx << 16) + regs.w.cx));
}

void VBE_Mode_Information (short Mode, VBE_ModeInfoBlock __far * a)
{
  PrepareRegisters();
  RMI.EAX=0x00004f01;               // Get SVGA-Mode Information
  RMI.ECX=Mode;
  RMI.ES=VbeModePool.segment;           // Segment of realmode data
  RMI.EDI=0;                        // offset of realmode data
  RMIRQ (0x10);
  _fmemcpy (a, MK_FP (VbeModePool.selector, 0), sizeof (VBE_ModeInfoBlock));
}

int VBE_IsModeLinear (short Mode)
{
#ifdef DISABLE_LFB
  return 0;
#else
  VBE_ModeInfoBlock a;
  VBE_Mode_Information (Mode, &a);
  return ((a.ModeAttributes & 128)==128);
#endif
}

#ifndef DISABLE_PM_EXTENSIONS
static void PM_SetDisplayStart (short x, short y)
{
  unsigned long  addr   = (y*BytesPerScanline+x);
  unsigned short loaddr = addr & 0xffff;
  unsigned short hiaddr = (addr>>16);
  #pragma aux asmSDS = "mov ax, 0x4f07" \
                       "xor ebx, ebx" \
                       "mov cx, loaddr" \
                       "mov dx, hiaddr" \
                       "call [pm_setdisplaystartcall]" \
                       modify [eax ebx ecx edx esi edi];
  asmSDS();
}

static void PM_SetBank (short bnk)
{
  if ( bnk==vbelastbank ) return;
  vbelastbank=bnk;
  #pragma aux asmSB = "mov ax, 0x4f05" \
                      "mov bx, 0" \
                      "mov dx, bnk" \
                      "call [pm_setwindowcall]" \
                      modify [eax ebx ecx edx esi edi];
  asmSB();
}
#endif

static void RM_SetDisplayStart (short x, short y)
{
  PrepareRegisters();
  RMI.EAX=0x00004f07;
  RMI.EBX=0;
  RMI.ECX=x;
  RMI.EDX=y;
  RMIRQ (0x10);
}

void VBE_SetMode (short Mode, int linear, int clear)
{
  VBE_ModeInfoBlock a;
  int rawmode;
  rawmode = Mode & 0x01ff;
  if ( linear) rawmode |= 1<<14;
  if (!clear)  rawmode |= 1<<15;
  PrepareRegisters();
  RMI.EAX=0x00004f02;               // Get SVGA-Mode Information
  RMI.EBX=rawmode;
  RMIRQ (0x10);
  VBE_Mode_Information (Mode, &a);
  BytesPerScanline = a.BytesPerScanline;
  BitsPerPixel = a.BitsPerPixel;
  /* Reinitalize the Protected Mode part. This hasn't been defined by VESA,
   * but it avoids a lot of errors and incompatibilities */
  VBE_InitPM ();
}

void VBE_Controller_Information  (VBE_VbeInfoBlock __far * a)
{
  PrepareRegisters();
  _fmemcpy (MK_FP (VbeInfoPool.selector, 0), a, 512);
  RMI.EAX=0x00004f00;               // Get SVGA-Information
  RMI.ES=VbeInfoPool.segment;       // Segment of realmode data
  RMI.EDI=0;                        // offset of realmode data
  RMIRQ (0x10);
  _fmemcpy (a, MK_FP (VbeInfoPool.selector, 0), 512);

  // Jetzt neu: Direktes Konvertieren der Realmode Pointer zu echten pointern
  a->OemStringPtr=(char*)((((unsigned long)a->OemStringPtr>>16)<<4)+(unsigned short)a->OemStringPtr);
  a->VideoModePtr=(unsigned short*)((((unsigned long)a->VideoModePtr>>16)<<4)+(unsigned short)a->VideoModePtr);
  a->OemVendorNamePtr=(char*)((((unsigned long)a->OemVendorNamePtr>>16)<<4)+(unsigned short)a->OemVendorNamePtr);
  a->OemProductNamePtr=(char*)((((unsigned long)a->OemProductNamePtr>>16)<<4)+(unsigned short)a->OemProductNamePtr);
  a->OemProductRevPtr=(char*)((((unsigned long)a->OemProductRevPtr>>16)<<4)+(unsigned short)a->OemProductRevPtr);
}

int VBE_Test (void)
{
  VBE_VbeInfoBlock VBEInfo;
  PrepareInfoBlock (&VBEInfo);
  VBE_Controller_Information (&VBEInfo);
  return (VBEInfo.vbeVersion>=0x200);
}

unsigned int VBE_VideoMemory (void)
{
  VBE_VbeInfoBlock VBEInfo;
  PrepareInfoBlock (&VBEInfo);
  VBE_Controller_Information (&VBEInfo);
  return (VBEInfo.TotalMemory*1024*64);
}

int VBE_FindMode (int xres, int yres, char bpp)
{
  int i;
  VBE_VbeInfoBlock VBEInfo;
  VBE_ModeInfoBlock Info;
  PrepareInfoBlock (&VBEInfo);
  VBE_Controller_Information (&VBEInfo);
  // First try to find the mode in the ControllerInfoBlock:
  for (i=0; VBEInfo.VideoModePtr[i]!=0xffff; i++ ) {
    VBE_Mode_Information (VBEInfo.VideoModePtr[i], &Info);
    if ((xres == Info.XResolution) &&
        (yres == Info.YResolution) &&
        (bpp == Info.BitsPerPixel)) return VBEInfo.VideoModePtr[i];
  }
  return -1;
}

char * VBE_GetVideoPtr (int mode)
{
  VBE_VbeInfoBlock VBEInfo;
  VBE_ModeInfoBlock ModeInfo;
  PrepareInfoBlock (&VBEInfo);
  VBE_Controller_Information (&VBEInfo);
  VBE_Mode_Information (mode, &ModeInfo);
  return (char *) DPMI_MAP_PHYSICAL (ModeInfo.PhysBasePtr, (VBEInfo.TotalMemory*64*1024));
}

void RM_SetBank (short bnk)
{
  if ( bnk==vbelastbank ) return;
  PrepareRegisters();
  RMI.EAX=0x00004f05;
  RMI.EBX=0;
  RMI.EDX=bnk;
  RMIRQ (0x10);
  vbelastbank=bnk;
}

short VBE_MaxBytesPerScanline (void)
{
  PrepareRegisters();
  RMI.EAX=0x00004f06;
  RMI.EBX=3;
  RMIRQ (0x10);
  return regs.w.bx;
}

void VBE_SetPixelsPerScanline (short Pixels)
{
  PrepareRegisters();
  RMI.EAX=0x00004f06;
  RMI.EBX=0;
  RMI.ECX=Pixels;
  RMIRQ (0x10);
  BytesPerScanline = (Pixels*BitsPerPixel/8);
}

void VBE_SetDACWidth (char bits)
{
  PrepareRegisters();
  RMI.EAX=0x00004f08;
  RMI.EBX=bits<<8;
  RMIRQ (0x10);
}

int VBE_8BitDAC (void)
{
  VBE_VbeInfoBlock VBEInfo;
  PrepareInfoBlock (&VBEInfo);
  VBE_Controller_Information (&VBEInfo);
  return (VBEInfo.Capabilities & 1 );
}

void VBE_InitPM (void)
{
  unsigned short *pm_pointer;
  VBE_SetDisplayStart = RM_SetDisplayStart;
  VBE_SetBank         = RM_SetBank;
#ifndef DISABLE_PM_EXTENSIONS
  PrepareRegisters();
  RMI.EAX=0x00004f0a;
  RMIRQ (0x10);
  if ( (RMI.EAX) == 0x004f ) {
    if (pmcode) free (pmcode);
    // get some memory to copy the stuff.
    pmcode = malloc (RMI.ECX  & 0x0000ffff);
    pm_pointer =   (unsigned short *) (((unsigned long )RMI.ES<<4)+(RMI.EDI));
    memcpy (pmcode, pm_pointer, (RMI.ECX  & 0x0000ffff));
    pm_pointer = (unsigned short *) pmcode;
    pm_setwindowcall =       (void *) (((unsigned long )RMI.ES<<4)+(RMI.EDI+pm_pointer[0]));
    pm_setdisplaystartcall = (void *) (((unsigned long )RMI.ES<<4)+(RMI.EDI+pm_pointer[1]));
    VBE_SetDisplayStart = PM_SetDisplayStart;
    VBE_SetBank         = PM_SetBank;
  }
#endif
}

void VBE_Init (void)
{
  unsigned short *pm_pointer;
  DPMI_AllocDOSMem (512/16, &VbeInfoPool);
  DPMI_AllocDOSMem (256/16, &VbeModePool);
  VBE_InitPM();
}

void VBE_Done (void)
{
  DPMI_FreeDOSMem (&VbeModePool);
  DPMI_FreeDOSMem (&VbeInfoPool);
  if (pmcode) free (pmcode);
}












