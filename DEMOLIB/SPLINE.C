/* craft / fudge */

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

//#include "salloc.h"



typedef struct spline_key *spline_ke;
typedef struct spline_key{
        spline_ke prev,next,j;   //j skal kun bruges i 'mainkey' til at vise den spline der blev beregnet v‘rdi fra sidst
        float   t;
        float   f,fm,a,b;
} spline_key;

static spline_key      *A_spline_key(spline_key *prev,spline_key *next,spline_key *j,float t,float f,float fm,float a,float b)
{
        spline_key      *k;
        assert(k=malloc(sizeof(spline_key)));

        k->prev=prev;
        k->next=next;
        k->j=j;
        k->t=t;
        k->f=f;
        k->fm=fm;
        k->a=a;
        k->b=b;

        return k;
}

static void    FREE_spline_key(spline_key *k)
{
        free(k);
}

//vmem_blockdata  spline_block;

spline_key* spline_new()
{
        return NULL;
}

void    spline_free(spline_key **sk1)
{
        spline_key      *sk,*skh;
        sk=(*sk1);
        while(sk!=NULL)
        {
                skh=sk->next;
//                vmem_freeblock(&spline_block,(vmem_block*)sk);
                FREE_spline_key(sk);
                sk=skh;
        }
}

void    spline_addkey(spline_key **sk1, float time, float value)
{
        float   dt,u,v,w,a,b;
        spline_key      *sk;
        sk=(*sk1);
        if(sk==NULL)
        {
//                (*sk1)=sk=(spline_key*)vmem_allocblock(&spline_block);
                (*sk1)=sk=A_spline_key(NULL,NULL,NULL,time,value,0,0,0);
                sk->j=sk;
        }
        else
        {
                while(sk->next!=NULL)sk=sk->next;
//                sk->next=(spline_key*)vmem_allocblock(&spline_block);
                sk=sk->next=A_spline_key(sk,NULL,sk,time,value,0,0,0);
        }

        sk->a=0;
        sk->b=0;
        sk->fm=0;

        if(sk->prev!=NULL)
        {
                if(sk->prev->prev!=NULL)
                {
                        dt=sk->prev->t-sk->prev->prev->t;
                        u=sk->prev->f-sk->prev->prev->f;
                        v=(sk->f-sk->prev->prev->f)/(sk->t-sk->prev->prev->t);
                        w=sk->prev->prev->fm;

                        // Beregning af a og b  i f(t)=a*t*t*t + b*t*t + w*t
                        // u=f(dt), v=fm(dt) w=fm(0) f(0)=0
                        a=(u-w*dt-0.5*dt*v+0.5*dt*w)/(-0.5*dt*dt*dt);
                        b=(u-a*dt*dt*dt-w*dt)/(dt*dt);

                        sk->prev->prev->fm=w;
                        sk->prev->prev->a=a;
                        sk->prev->prev->b=b;



                        dt=sk->t-sk->prev->t;
                        u=value-sk->prev->f;
                        v=0;
                        w=(sk->f-sk->prev->prev->f)/(sk->t-sk->prev->prev->t);

                        // Beregning af a og b  i f(t)=a*t*t*t + b*t*t + w*t
                        // u=f(dt), v=fm(dt) w=fm(0) f(0)=0
                        a=(u-w*dt-0.5*dt*v+0.5*dt*w)/(-0.5*dt*dt*dt);
                        b=(u-a*dt*dt*dt-w*dt)/(dt*dt);

                        sk->prev->fm=w;
                        sk->prev->a=a;
                        sk->prev->b=b;
                }
                else
                {
                        dt=time-sk->prev->t;
                        u=value-sk->prev->f;
                        v=0;
                        w=0;

                        // Beregning af a og b  i f(t)=a*t*t*t + b*t*t + w*t
                        // u=f(dt), v=fm(dt) w=fm(0) f(0)=0
                        a=(u-w*dt-0.5*dt*v+0.5*dt*w)/(-0.5*dt*dt*dt);
                        b=(u-a*dt*dt*dt-w*dt)/(dt*dt);

                        sk->prev->fm=0;
                        sk->prev->a=a;
                        sk->prev->b=b;


                }
        }
}

void    spline_calc(spline_key *sk)
{

}

float   spline_value(spline_key *sk, float time)
{
        spline_key      *s;

        if(sk==NULL){printf("NO KEYS..\n");return 0;}

        s=sk->j;

        while((time<s->t)&(s->prev!=NULL))s=s->prev;
        while(s->next!=NULL) if(time>s->next->t) s=s->next;else break;

        sk->j=s;

        if(time<s->t) return s->f;

        else return (((time-s->t)*s->a+s->b)*(time-s->t)+s->fm)*(time-s->t)+s->f;
}

void    spline_printfkey(spline_key *sk)
{
        printf("Prev. key:%x\n",sk->prev);
        printf("Next  key:%x\n",sk->next);
        printf("Time     :%i\n",sk->t);
        printf("Value    :%f\n",sk->f);
        printf("Difvalue :%f\n",sk->fm);
        printf("Coef. A  :%f\n",sk->a);
        printf("Coef. B  :%f\n",sk->b);
        printf("\n");
}

void    spline_printf(spline_key *sk)
{
        if(sk==NULL){printf("NO SPLINE\n");return;}

        while(sk!=NULL)
        {
                spline_printfkey(sk);
                sk=sk->next;
        }
}

void    spline_init(int n)
{
//        vmem_initblocks(&spline_block,sizeof(spline_key),n);
}

void    spline_deinit()
{
//        vmem_deinitblocks(&spline_block);
}

void    spline_ref(spline_key **a, spline_key **b)
{
        // Lad *b referere til splinen i stedet for *a

        *b=*a;
        *a=NULL;
}
