#ifndef qspline_h_
#define qspline_h_

#include <stdio.h>
#include "structs.h"
#include "quat.h"

qspline_key* qspline_new();
void    qspline_free(qspline_key **sk1);
void    qspline_addkey(qspline_key **sk1, float time, quat value);
void    qspline_calc(qspline_key *sk);
quat    qspline_value(qspline_key *sk, float time);
void    qspline_printfkey(qspline_key *sk);
void    qspline_printf(qspline_key *sk);
void    qspline_init(int n);
void    qspline_deinit();
void    qspline_ref(qspline_key **a, qspline_key **b);

#endif
