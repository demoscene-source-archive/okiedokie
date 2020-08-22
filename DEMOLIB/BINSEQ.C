/* craft / fudge */

#include <stdlib.h>

#include "binseq.h"

void    bs_error()
{
        printf("BinSeq.c error! (out of bytes?)\n");
        exit(1);
}

void    bs_init(bs_sequence *seq, char* list, int length)
{
        seq->list=list;
        seq->length=length;
        seq->set_current=0;
        seq->set_bit=0;
        seq->set_last=0;
        seq->get_current=0;
        seq->get_bit=1;
        seq->get_last=0;
}

void    bs_set(bs_sequence *seq, int bit)
{
        if(seq->set_bit==bit)
        {
                seq->set_last++;
                if(seq->set_last==256)
                {
                        if(seq->set_current>=seq->length)bs_error();
                        seq->list[seq->set_current]=255;seq->set_current++;
                        if(seq->set_current>=seq->length)bs_error();
                        seq->list[seq->set_current]=0;seq->set_current++;
                        seq->set_last-=255;
                }
        }
        else
        {
                if(seq->set_current>=seq->length)bs_error();
                seq->list[seq->set_current]=seq->set_last;seq->set_current++;
                seq->set_last=1;
                seq->set_bit=1-seq->set_bit;
        }
        if(seq->set_current>=seq->length)bs_error();
        seq->list[seq->set_current]=255;
}

int     bs_get(bs_sequence *seq)
{
        while(seq->get_last==0)
        {
                if(seq->get_current>=seq->length)bs_error();
                seq->get_last=seq->list[seq->get_current];
                seq->get_current++;
                seq->get_bit=1-seq->get_bit;
        }
        seq->get_last--;

        return seq->get_bit;
}

void    bs_getreset(bs_sequence *seq)
{
        seq->get_current=0;
        seq->get_bit=1;
        seq->get_last=0;
}

void    bs_load(bs_sequence *seq, FILE *fp)
{
        int     t;

        seq->set_current=f_getdword(fp);
        seq->set_bit=f_getdword(fp);
        seq->set_last=f_getdword(fp);

        if(seq->set_current>seq->length)bs_error();

        for(t=0;t<seq->set_current;t++)
        {
                seq->list[t]=f_getbyte(fp);
        }

        bs_getreset(seq);
}

void    bs_save(bs_sequence *seq, FILE *fp)
{
        int     t;

        f_putdword(seq->set_current,fp);
        f_putdword(seq->set_bit,fp);
        f_putdword(seq->set_last,fp);

        for(t=0;t<seq->set_current;t++)
        {
                f_putbyte(seq->list[t],fp);
        }
}



