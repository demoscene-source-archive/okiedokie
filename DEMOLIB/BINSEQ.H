#ifndef __binseq_h
#define __binseq_h

#include    <stdio.h>
#include    "filer.h"

typedef struct bs_sequence
{
    unsigned char*  list;
    int             length;
    int             set_current,set_bit,set_last;
    int             get_current,get_bit,get_last;
} bs_sequence;


void    bs_init(bs_sequence *seq, char* list, int length);
void    bs_set(bs_sequence *seq, int bit);
int     bs_get(bs_sequence *seq);
void    bs_getreset(bs_sequence *seq);
void    bs_load(bs_sequence *seq, FILE *fp);
void    bs_save(bs_sequence *seq, FILE *fp);


#endif
