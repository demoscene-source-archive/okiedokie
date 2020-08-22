

#ifndef __soopa_h
#define __soopa_h

#include "main.h"

void wire_init1(void);
void wire_init2(void);
layer wire_main(int useframe);
void wire_deinit(void);

void weknows_init1(void);
void weknows_init2(void);
layer weknows_main(int useframe);
void weknows_deinit(void);

void blob_init1(void);
void blob_init2(void);
layer blob_main(int useframe);
void blob_deinit(void);

#endif
