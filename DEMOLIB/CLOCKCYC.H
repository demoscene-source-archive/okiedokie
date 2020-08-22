#ifndef CLOCKCYC

#define CLOCKCYC

typedef  struct cc_timer *cc_timerp;
typedef struct cc_timer
{
//        __int64   amount,last;
        int   amountlo,amounthi,lastlo,lasthi;
        cc_timerp next;
        char     *remark;
}cc_timer;

void    cc_timeron(cc_timer* t);
#pragma aux cc_timeron=\
"db 0x0f"\
"db 0x31"\
"mov [esi+8],eax"\
"mov [esi+12],edx"\
parm[esi]\
modify[edx eax];
void    cc_timeroff(cc_timer* t );
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
void    cc_init();
void    cc_addtimer(cc_timer *t, char* rem);
void    cc_print();
void    cc_printperframe(int frames,int cpu,int scrs);
void    cc_deinit();
void    cc_timeroff(cc_timer* t );
void    cc_timeron(cc_timer* t);
void    cc_autodetect();

#endif
