/*      DEMO.C
 *
 * MIDAS megademo
 *
 * Copyright 1996 Petteri Kangaslampi
*/

/* craft made M_*() by moving code from the original file into theese procedures. Some of the original procedures were deleted. Get the original MIDAS at www.hornet.org */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include "midasdll.h"
#include "vga.h"

#include "mdemo.h"

#define ATTEMPT_DETECT


/* Music playing position information - updated by UpdateInfo: */
unsigned        position;               /* Current position */
unsigned        pattern;                /* Current pattern number */
unsigned        row;                    /* Current row number */
int             syncInfo;               /* Music synchronization info */

/* Frame counter variable - note VOLATILE! */
volatile unsigned   frameCount;


unsigned refreshRate;        /* Display refresh rate */
MIDASmodule module;                 /* Der Module */


void MIDAS_CALL prevr(void)
{
    frameCount++;
}




void MIDASerror(void)
{
    vgaSetMode(3);
    printf("MIDAS error: %s\n", MIDASgetErrorMessage(MIDASgetLastError()));
    MIDASclose(); /*!!!!!*/
    exit(EXIT_FAILURE);
}

void WaitFrame(void)
{
    unsigned    old;

    /* Playing music - wait for frame counter to change: */
    old = frameCount;
    while ( old == frameCount );
}




void MIDAS_CALL UpdateInfo(void)
{
    static MIDASplayStatus status;

    if ( !MIDASgetPlayStatus(&status) )MIDASerror();

    position = status.position;
    pattern = status.pattern;
    row = status.row;
    syncInfo = status.syncInfo;
}

void MIDAS_CALL SyncCallback(unsigned syncNum, unsigned position, unsigned row)
{
    /* Prevent warnings: */
    position = position;
    row = row;

    /* Check if the infobyte is interesting - do something only when command
       "W42" is encountered: */
        if ( syncNum == 0x42 )
        {
        }
}

void    M_init1(int force)
{
        MIDASstartup();

        //Make forced sound config by -setup

        if((force)||(!MIDASdetectSoundCard()))
        {
                if(!MIDASconfig())
                {
                        /* Configuration failed. Check if it is an error - if yes, report,
                           otherwise exit normally: */

                        if(MIDASgetLastError())
                        {
                                MIDASerror();
                        }
                        else
                        {
                                printf("User exit!\n");
                                return;
                        }
                }
        }
}

void    M_init2(char* filename)
{
        refreshRate=MIDASgetDisplayRefreshRate();


        /* Check if we got the refresh rate: */
        if(refreshRate==0)
        {
                //Set display mode to text!
                vgaSetMode(3);

                printf("Warning! Unable to synchronize to display refresh!\n"
                        "This can lead to problems in music output and flickering.\n"
                        "This usually happens only when running under Windows 95 - "
                        "please consider\nrunning DOS programs in DOS instead.\n"
                        "Press Esc to quit or any other key to continue\n");

                if(getch()==27) return;
                refreshRate=70000; /* default 70Hz */
        }

        if(!MIDASinit()) MIDASerror();
        if((module=MIDASloadModule(filename))==NULL) MIDASerror();
        if(!MIDASsetTimerCallbacks(refreshRate, TRUE, &prevr, NULL, NULL)) MIDASerror();

        if(!MIDASplayModule(module,0)) MIDASerror();
        if(!MIDASsetMusicSyncCallback(&SyncCallback)) MIDASerror();
}

void    M_deinit(void)
{
        if(!MIDASsetMusicSyncCallback(NULL)) MIDASerror();
        if(!MIDASstopModule(module)) MIDASerror();
        if(!MIDASfreeModule(module)) MIDASerror();
        if(!MIDASremoveTimerCallbacks()) MIDASerror();
        if(!MIDASclose()) MIDASerror();
}

void    M_setposition(int pos)
{
        MIDASsetPosition(pos);
}

void    M_updateinfo(void)
{
        UpdateInfo();
}

void    M_waitframe(void)
{
        WaitFrame();
}
