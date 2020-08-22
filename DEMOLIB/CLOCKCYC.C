/* craft / fudge */

#ifndef CLOCKCYC
#define CLOCKCYC

#include <stdio.h>
#include <time.h>

typedef  struct cc_timer *cc_timerp;

typedef struct cc_timer
{
        __int64   amount,last;
        cc_timerp next;
        char     *remark;
}cc_timer;

cc_timerp timers=0;
int     cc_autodetected=0;
int     cc_cpuspeed=0;
int     cc_screenrefreshrate=0;

cc_timer maintimer;

extern void    cc_timeron(cc_timer* t);
#pragma aux cc_timeron=\
"db 0x0f"\
"db 0x31"\
"mov [esi+8],eax"\
"mov [esi+12],edx"\
parm[esi]\
modify[edx eax];
/*
"push eax"\
"pop eax"\
"push edx"\
"pushad"\
"popad"\
"pop edx"\   */


extern void    cc_timeroff(cc_timer* t );
#pragma aux cc_timeroff=\
"db 0x0f"\
"db 0x31"\
"mov ebx,[esi+8]"\
"mov ecx,[esi+12]"\
"sub eax,ebx"\
"mov ebx,[esi+0]"\
"sbb edx,ecx"\
"mov ecx,[esi+4]"\
"add eax,ebx"\
"adc edx,ecx"\
"mov [esi+0],eax"\
"mov [esi+4],edx"\
parm[esi]\
modify[eax ebx ecx edx];

extern void    cc_vret(void);
#pragma aux cc_vret=\
"        pushad             "\
"        mov     dx,3dah    "\
"@@L1:                      "\
"        in      al,dx      "\
"        test    al,8       "\
"        jnz     @@L1       "\
"@@L2:                      "\
"        in      al,dx      "\
"        test    al,8       "\
"        jz      @@L2       "\
"        popad              "\
parm[]\
modify[];


void    cc_init()
{
        cc_addtimer(&maintimer,"main");
        cc_timeron(&maintimer);
}

void    cc_deinit()
{
        cc_timeroff(&maintimer);
        timers=NULL;
}

void    cc_addtimer(cc_timer *t, char* rem)
{
        t->next=timers;
        timers=t;
        t->remark=rem;
        t->amount=0;
}

void    cc_print()
{
        float  sum=0;
        cc_timer *t;

        t=timers;

        cc_timeroff(&maintimer);
        cc_timeron(&maintimer);

        sum=maintimer.amount;

        printf("        TIMER           Clockcycles       Relative\n");
        while(t!=0)
        {
                printf("%10s %22.0f            %3.2f %%\n",t->remark,(double)t->amount,100.0*((double)t->amount)/sum);
                t=t->next;
        }
        printf("Total number of clockcycles:%.0f\n",sum);
}

void    cc_printperframe(int frames,int cpu,int scrs)
{
        double  sum=0;
        cc_timer *t;

        cc_timeroff(&maintimer);
        cc_timeron(&maintimer);


        if(cc_autodetected)
        {
                cpu=((double)cc_cpuspeed/1000000)+0.5;
                scrs=cc_screenrefreshrate;
        }

        printf("\nPentium clockcycle report\n");
        printf("        ...by Craft/fudGe\n\n");
        printf("Assumed Screen-Refresh Rate:  %5i frames/sec.\n",scrs);
        printf("Assumed cpu-speed:            %5i         MHz\n",cpu);
        sum=maintimer.amount;
        printf("Total number of clockcycles:     %.0f\n",sum);
        printf("Frames per second:               %f\n\n",(float)frames/(sum/((float)cpu*1000000)));


        t=timers;


        sum=maintimer.amount/frames;

        printf("TIMER                                CLOCKCYCLES RELATIVE RASTERTIME(frames)\n");
        while(t!=0)
        {
                printf(" %-30s %16.0f %6.2f %%          %6.2f\n",t->remark,(double)t->amount/frames,100.0*((double)t->amount/frames)/sum, (double) t->amount / ( (float)cpu*1000000 ) /frames *(float)scrs);
                t=t->next;
        }
        printf("\n");
}

void    cc_autodetect()
{
        cc_timer timer;
        int     t0,t1,t2,t3;
        float   secs;


/*        clock();
        cc_vret();
        timer.amount=0;
        t1=clock();
        cc_timeron(&timer);
        for(t=0;t<20;t++)cc_vret();
        t1-=clock();
        cc_timeroff(&timer);

        secs=((float)-t1)/((float)CLOCKS_PER_SEC);

        cc_cpuspeed=(timer.amount/secs);
        cc_screenrefreshrate=(20/secs);
  */

        clock();
        timer.amount=0;
        t0=t1=clock();
        while(t1==t0)t2=t1=clock();
        cc_timeron(&timer);
        while(t1==t2)t3=t2=clock();
        while(t2==t3)t3=clock();
        cc_timeroff(&timer);

        cc_cpuspeed=timer.amount/((double)(t3-t1)/CLOCKS_PER_SEC);


        timer.amount=0;
        cc_vret();
        cc_timeron(&timer);
        cc_vret();
        cc_timeroff(&timer);

        cc_screenrefreshrate=((double)cc_cpuspeed/timer.amount)+0.5;


        cc_autodetected=1;

        printf("cps:%i\n",(int)CLOCKS_PER_SEC);
}

#endif
