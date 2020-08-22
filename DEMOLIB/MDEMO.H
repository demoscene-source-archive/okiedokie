#ifndef _mdemo_h_
#define _mdemo_h_


/* Music playing position information - updated by M_updateinfo(UpdateInfo): */
extern unsigned        position;               /* Current position */
extern unsigned        pattern;                /* Current pattern number */
extern unsigned        row;                    /* Current row number */
extern int             syncInfo;               /* Music synchronization info */

/* Frame counter variable - note VOLATILE! */
extern volatile unsigned   frameCount;


extern unsigned refreshRate;        /* Display refresh rate */

void    M_waitframe(void);
void    M_updateinfo(void);
void    M_setposition(int pos);
void    M_init1(int force);
void    M_init2(char* filename);
void    M_deinit(void);

#endif
