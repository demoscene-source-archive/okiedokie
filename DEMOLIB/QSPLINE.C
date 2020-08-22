/* craft / fudge */

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
//#include "salloc.h"
#include "qspline.h"



static qspline_key      *A_qspline_key(qspline_key *prev,qspline_key *next,qspline_key *j,float t, quat q, quat a)
{
        qspline_key      *k;
        assert(k=malloc(sizeof(qspline_key)));

        k->prev=prev;
        k->next=next;
        k->j=j;
        k->t=t;
        k->q=q;
        k->a=a;

        return k;
}

static void    FREE_qspline_key(qspline_key *k)
{
        free(k);
}


qspline_key* qspline_new()
{
        return NULL;
}

void    qspline_free(qspline_key **sk1)
{
        qspline_key      *sk,*skh;
        sk=(*sk1);
        while(sk!=NULL)
        {
                skh=sk->next;
                FREE_qspline_key(sk);
                sk=skh;
        }
}

void    qspline_addkey(qspline_key **sk1, float time, quat value)
{
        float   dt,u,v,w,a,b;
        qspline_key      *sk;
        sk=(*sk1);
        if(sk==NULL)
        {
                (*sk1)=sk=A_qspline_key(NULL,NULL,NULL,time,value,value);
                sk->j=sk;
        }
        else
        {
                while(sk->next!=NULL)sk=sk->next;
                sk=sk->next=A_qspline_key(sk,NULL,sk,time,value,value);
        }

        if(sk->prev!=NULL)
        {
                // Er der kun indsat en key, skal der ikke g›res noget

                // Ellers skal den n‘stsidste key regnes om, med mindre det var den f›rste
                if(sk->prev->prev!=NULL)
                {
                        sk->prev->a=quat_quadranglepoint(sk->prev->prev->q,sk->prev->q,sk->q);
                }
                else
                {
                }
        }
}

void    qspline_calc(qspline_key *sk)
{

}

quat    qspline_value(qspline_key *sk, float time)
{
        qspline_key      *s;

        if(sk==NULL){printf("NO KEYS..\n");return quat_makerotate(0,1,0,0);}

        s=sk->j;

        while((time<s->t)&&(s->prev!=NULL))s=s->prev;
        while(s->next!=NULL) if(time>s->next->t) s=s->next;else break;

        sk->j=s;

        if(time<s->t) return s->q;
        else if (s->next==NULL) return s->q;
        else return quat_squad(s->q,s->a,s->next->a,s->next->q,(time-s->t)/(s->next->t-s->t));
}

void    qspline_printfkey(qspline_key *sk)
{
        printf("Prev. key:%x\n",sk->prev);
        printf("Next  key:%x\n",sk->next);
        printf("Time     :%i\n",sk->t);
        printf("Q: ");quat_print(sk->q);
        printf("A: ");quat_print(sk->a);
        printf("\n");
}

void    qspline_printf(qspline_key *sk)
{
        if(sk==NULL){printf("NO SPLINE\n");return;}

        while(sk!=NULL)
        {
                qspline_printfkey(sk);
                sk=sk->next;
        }
}

void    qspline_init(int n)
{
}

void    qspline_deinit()
{
}

void    qspline_ref(qspline_key **a, qspline_key **b)
{
        // Lad *b referere til splinen i stedet for *a

        *b=*a;
        *a=NULL;
}
