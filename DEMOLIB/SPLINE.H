#ifndef spline_h_
#define spline_h_

#include <stdio.h>

#include    "structs.h"
/*
typedef struct spline_key *spline_ke;
typedef struct spline_key{
        spline_ke prev,next,j;   //j skal kun bruges i 'mainkey' til at vise den spline der blev beregnet v‘rdi fra sidst
        float   t;
        float   f,fm,a,b;
} spline_key;
  */
spline_key* spline_new();
void    spline_free(spline_key **sk1);
void    spline_addkey(spline_key **sk1, float time, float value);
void    spline_calc(spline_key *sk);
float   spline_value(spline_key *sk, float time);
void    spline_printfkey(spline_key *sk);
void    spline_printf(spline_key *sk);
void    spline_init(int n);
void    spline_deinit();
void    spline_ref(spline_key **a, spline_key **b);

#endif
