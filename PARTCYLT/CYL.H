#ifndef __trace_h
#define __trace_h

void trace_init1(void);
void trace_init2(void);
layer trace_main(int useframe);
void trace_deinit(void);

extern float trace_posx,trace_posy,trace_posz,trace_rotx,trace_roty,trace_rotz;

#endif
