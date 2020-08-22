/*      midasdll.h
 *
 * MIDAS DLL programming interface
 *
 * $Id: midasdll.h,v 1.12 1997/03/09 19:13:01 pekangas Exp $
 *
 * Copyright 1996,1997 Housemarque Inc.
 *
 * This file is part of the MIDAS Sound System, and may only be
 * used, modified and distributed under the terms of the MIDAS
 * Sound System license, LICENSE.TXT. By continuing to use,
 * modify or distribute this file you indicate that you have
 * read the license and understand and accept it fully.
*/


#ifndef __midasdll_h
#define __midasdll_h


/* This is a kluge, but necessary as Watcom C sucks: */
#ifdef EXPORT_IN_MIDASDLL_H

#ifdef __WC32__
    #define _FUNC(x) x __export __stdcall
    #define MIDAS_CALL __cdecl
#else
    #define _FUNC(x) __declspec(dllexport) x __stdcall
    #define MIDAS_CALL __cdecl
#endif

#else
    #if defined(__LINUX__) || defined(__linux__) || defined(__DJGPP__)
        #define _FUNC(x) x
        #define MIDAS_CALL
    #else
        #ifdef __DOS__
            #define _FUNC(x) x cdecl
            #define MIDAS_CALL __cdecl
        #else
            #define _FUNC(x) x __stdcall
            #define MIDAS_CALL __cdecl
        #endif
    #endif
#endif


#if defined(__WATCOMC__) && defined(__cplusplus)
/* Disable to annoying Watcom C++ warnings - I have no idea how to get around
   these without breaking Visual C compatibility: */
#pragma warning 604 9
#pragma warning 594 9
#endif


/* We'll need to define DWORD, BOOL, TRUE and FALSE if someone hasn't
   done that before. For now, we'll just assume that if no-one has defined
   TRUE we need to define everything. There definitions are compatible with
   windows.h. If something else in your system defines these differently,
   things should still work OK as long as FALSE is 0, TRUE is nonzero and
   DWORD is 32-bit. Take care that you don't compare BOOLs like "bool == TRUE"
   in that case though, just use "bool".

   THIS IS UGLY AND MAY NEED FIXING!
   ---------------------------------
*/

#ifndef TRUE
#define TRUE 1
#define FALSE 0
typedef int BOOL;
typedef unsigned long DWORD;
#endif /* ifndef TRUE */



enum MIDASoptions
{
    MIDAS_OPTION_NONE = 0,
    MIDAS_OPTION_MIXRATE,
    MIDAS_OPTION_OUTPUTMODE,
    MIDAS_OPTION_MIXBUFLEN,
    MIDAS_OPTION_MIXBUFBLOCKS,
    MIDAS_OPTION_DSOUND_MODE,
    MIDAS_OPTION_DSOUND_HWND,
    MIDAS_OPTION_DSOUND_OBJECT,
    MIDAS_OPTION_DSOUND_BUFLEN,
    MIDAS_OPTION_16BIT_ULAW_AUTOCONVERT
};


enum MIDASmodes
{
    MIDAS_MODE_NONE = 0,
    MIDAS_MODE_MONO = 1,
    MIDAS_MODE_STEREO = 2,
    MIDAS_MODE_8BIT = 4,
    MIDAS_MODE_16BIT = 8,
    MIDAS_MODE_8BIT_MONO = MIDAS_MODE_8BIT | MIDAS_MODE_MONO,
    MIDAS_MODE_8BIT_STEREO = MIDAS_MODE_8BIT | MIDAS_MODE_STEREO,
    MIDAS_MODE_16BIT_MONO = MIDAS_MODE_16BIT | MIDAS_MODE_MONO,
    MIDAS_MODE_16BIT_STEREO = MIDAS_MODE_16BIT | MIDAS_MODE_STEREO
};



enum MIDASsampleTypes
{
    MIDAS_SAMPLE_NONE = 0,
    MIDAS_SAMPLE_8BIT_MONO = 1,
    MIDAS_SAMPLE_16BIT_MONO = 2,
    MIDAS_SAMPLE_8BIT_STEREO = 3,
    MIDAS_SAMPLE_16BIT_STEREO = 4,
    MIDAS_SAMPLE_ADPCM_MONO = 5,
    MIDAS_SAMPLE_ADPCM_STEREO = 6,
    MIDAS_SAMPLE_ULAW_MONO = 7,
    MIDAS_SAMPLE_ULAW_STEREO = 8
};



enum MIDASloop
{
    MIDAS_LOOP_NO = 0,
    MIDAS_LOOP_YES
};


enum MIDASpanning
{
    MIDAS_PAN_LEFT = -64,
    MIDAS_PAN_MIDDLE = 0,
    MIDAS_PAN_RIGHT = 64,
    MIDAS_PAN_SURROUND = 0x80
};


enum MIDASchannels
{
    MIDAS_CHANNEL_AUTO = 0xFFFF
};


enum MIDASdsoundModes
{
    MIDAS_DSOUND_DISABLED = 0,
    MIDAS_DSOUND_STREAM,
    MIDAS_DSOUND_PRIMARY,
    MIDAS_DSOUND_FORCE_STREAM
};



typedef struct
{
    char        songName[32];
    unsigned    songLength;
    unsigned    numPatterns;
    unsigned    numInstruments;
    unsigned    numChannels;
} MIDASmoduleInfo;



typedef struct
{
    char        instName[32];
} MIDASinstrumentInfo;



typedef struct
{
    unsigned    position;
    unsigned    pattern;
    unsigned    row;
    int         syncInfo;
} MIDASplayStatus;


typedef void* MIDASmodule;
typedef DWORD MIDASsample;
typedef DWORD MIDASsamplePlayHandle;
typedef void* MIDASstreamHandle;


#ifdef __cplusplus
extern "C" {
#endif

_FUNC(int)      MIDASgetLastError(void);
_FUNC(char*)    MIDASgetErrorMessage(int errorCode);

_FUNC(DWORD)    MIDASgetDisplayRefreshRate(void);

_FUNC(BOOL)     MIDASstartup(void);
_FUNC(BOOL)     MIDASdetectSD(void);
_FUNC(BOOL)     MIDASdetectSoundCard(void);
_FUNC(BOOL)     MIDASconfig(void);
_FUNC(BOOL)     MIDASloadConfig(char *fileName);
_FUNC(BOOL)     MIDASsaveConfig(char *fileName);
_FUNC(BOOL)     MIDASinit(void);
_FUNC(BOOL)     MIDASsetOption(int option, DWORD value);
_FUNC(BOOL)     MIDASclose(void);
_FUNC(BOOL)     MIDASsuspend(void);
_FUNC(BOOL)     MIDASresume(void);
_FUNC(BOOL)     MIDASopenChannels(int numChannels);
_FUNC(BOOL)     MIDAScloseChannels(void);
_FUNC(BOOL)     MIDASsetAmplification(DWORD amplification);
_FUNC(BOOL)     MIDASstartBackgroundPlay(DWORD pollRate);
_FUNC(BOOL)     MIDASstopBackgroundPlay(void);
_FUNC(BOOL)     MIDASpoll(void);
_FUNC(char*)    MIDASgetVersionString(void);
_FUNC(BOOL)     MIDASsetTimerCallbacks(DWORD rate, BOOL displaySync,
				       void (MIDAS_CALL *preVR)(),
				       void (MIDAS_CALL *immVR)(),
				       void (MIDAS_CALL *inVR)());
_FUNC(BOOL)     MIDASremoveTimerCallbacks(void);

_FUNC(MIDASmodule) MIDASloadModule(char *fileName);
_FUNC(BOOL)     MIDASplayModule(MIDASmodule module, int numEffectChannels);
_FUNC(BOOL)     MIDASstopModule(MIDASmodule module);
_FUNC(BOOL)     MIDASfreeModule(MIDASmodule module);

_FUNC(BOOL)     MIDASgetPlayStatus(MIDASplayStatus *status);
_FUNC(BOOL)     MIDASsetPosition(int newPosition);
_FUNC(BOOL)     MIDASsetMusicVolume(unsigned volume);
_FUNC(BOOL)     MIDASgetModuleInfo(MIDASmodule module, MIDASmoduleInfo *info);
_FUNC(BOOL)     MIDASgetInstrumentInfo(MIDASmodule module, int instNum,
                    MIDASinstrumentInfo *info);
_FUNC(BOOL)     MIDASsetMusicSyncCallback(void (MIDAS_CALL *callback)
                                          (unsigned syncInfo,
                                           unsigned position, unsigned row));

_FUNC(MIDASsample) MIDASloadRawSample(char *fileName, int sampleType,
                    int loopSample);
_FUNC(BOOL)         MIDASfreeSample(MIDASsample sample);
_FUNC(BOOL)         MIDASsetAutoEffectChannels(unsigned firstChannel,
                        unsigned numChannels);
_FUNC(MIDASsamplePlayHandle) MIDASplaySample(MIDASsample sample,
                        unsigned channel, int priority, unsigned rate,
                        unsigned volume, int panning);
_FUNC(BOOL)     MIDASstopSample(MIDASsamplePlayHandle sample);
_FUNC(BOOL)     MIDASsetSampleRate(MIDASsamplePlayHandle sample,
                    unsigned rate);
_FUNC(BOOL)     MIDASsetSampleVolume(MIDASsamplePlayHandle sample,
                    unsigned volume);
_FUNC(BOOL)     MIDASsetSamplePanning(MIDASsamplePlayHandle sample,
                    int panning);
_FUNC(BOOL)     MIDASsetSamplePriority(MIDASsamplePlayHandle sample,
                    int priority);

_FUNC(MIDASstreamHandle) MIDASplayStreamFile(unsigned channel, char *fileName,
                    unsigned sampleType, unsigned sampleRate,
                    unsigned bufferLength, int loopStream);
_FUNC(BOOL)     MIDASstopStream(MIDASstreamHandle stream);

_FUNC(MIDASstreamHandle) MIDASplayStreamPolling(unsigned channel,
                    unsigned sampleType, unsigned sampleRate,
                    unsigned bufferLength);
_FUNC(unsigned) MIDASfeedStreamData(MIDASstreamHandle stream,
                    unsigned char *data, unsigned numBytes, BOOL feedAll);

_FUNC(BOOL)     MIDASsetStreamRate(MIDASstreamHandle stream, unsigned rate);
_FUNC(BOOL)     MIDASsetStreamVolume(MIDASstreamHandle stream,
                    unsigned volume);
_FUNC(BOOL)     MIDASsetStreamPanning(MIDASstreamHandle stream, int panning);




#ifdef __cplusplus
}
#endif




#endif


/*
 * $Log: midasdll.h,v $
 * Revision 1.12  1997/03/09 19:13:01  pekangas
 * Added the possibility to turn off u-law autoconvert
 *
 * Revision 1.11  1997/03/05 16:49:49  pekangas
 * Added timer functions to DLL, some other minor modifications
 *
 * Revision 1.10  1997/02/27 16:03:36  pekangas
 * Added DJGPP support
 *
 * Revision 1.9  1997/02/20 19:49:40  pekangas
 * Added u-law sample type
 *
 * Revision 1.8  1997/02/19 20:45:09  pekangas
 * Added functions MIDASsuspend() and MIDASresume()
 *
 * Revision 1.7  1997/02/12 17:18:37  pekangas
 * Added MIDASsetAmplification()
 *
 * Revision 1.6  1997/02/12 16:28:12  pekangas
 * Added ADPCM sample type
 *
 * Revision 1.5  1997/02/06 20:58:20  pekangas
 * Added DirectSound support - new files, errors, and global flags
 *
 * Revision 1.4  1997/01/16 18:41:59  pekangas
 * Changed copyright messages to Housemarque
 *
 * Revision 1.3  1997/01/16 18:26:27  pekangas
 * Added numerous new functions
 *
 * Revision 1.2  1996/09/28 08:12:40  jpaana
 * Fixed for Linux
 *
 * Revision 1.1  1996/09/25 18:38:12  pekangas
 * Initial revision
 *
*/
