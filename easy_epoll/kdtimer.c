#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include "kdtimer.h"

#ifndef EXPORT
#define EXPORT static inline
#endif

#define SHIFT_BITS 6
#define TVN_BITS 8
#define TVR_BITS 10
#define TVN_SIZE (1 << TVN_BITS)//2^8
#define TVR_SIZE (1 << TVR_BITS)//1024
#define TVN_MASK (TVN_SIZE - 1)//2^8 - 1
#define TVR_MASK (TVR_SIZE - 1)//1023

struct kdtimer_vec {
    int index;
    struct list_head vec[TVN_SIZE];
};

struct kdtimer_vec_root {
    int index;
    struct list_head vec[TVR_SIZE];
};

#define NOOF_SLTVECS 3

struct timer_base {
    struct kdtimer_vec_root sltv1;
    struct kdtimer_vec sltv2;
    struct kdtimer_vec sltv3;
    struct kdtimer_vec* sltvecs[3];
    unsigned long kdtimer_jiffies;
};

static struct timer_base* t_base = 0;

static inline void init_kdtimer_vecs(timer_base_t* base)
{
    int i;

    base->sltv1.index = 0;
    base->sltv2.index = 0;
    base->sltv3.index = 0;
    for (i = 0; i < TVN_SIZE; i++) {
        INIT_LIST_HEAD((struct list_head*)base->sltv3.vec + i);
        INIT_LIST_HEAD(base->sltv2.vec + i);
    }
    for (i = 0; i < TVR_SIZE; i++)
        INIT_LIST_HEAD(base->sltv1.vec + i);

    base->sltvecs[0] = (struct kdtimer_vec*)&base->sltv1;
    base->sltvecs[1] = &base->sltv2;
    base->sltvecs[2] = &base->sltv3;
	base->kdtimer_jiffies = 0;
}

static inline void internal_add_kdtimer(timer_base_t* base, kdtimer_t* timer)
{
    unsigned long expires = timer->expires;
    unsigned long idx = expires - base->kdtimer_jiffies;
    struct list_head* vec;
    if (idx < 1 << (SHIFT_BITS + TVR_BITS)) {
        int i = (expires >> SHIFT_BITS) & TVR_MASK;
        vec = base->sltv1.vec + i;
    }
    else if (idx < 1 << (SHIFT_BITS + TVR_BITS + TVN_BITS)) {
        int i = (expires >> (SHIFT_BITS + TVR_BITS)) & TVN_MASK;
        vec = base->sltv2.vec + i;
    }
    else if ((signed long)idx < 0) {
        // already expired
        vec = base->sltv1.vec + base->sltv1.index;
    }
    else if (idx <= 0xffffffffUL) {
        int i = (expires >> (SHIFT_BITS + TVR_BITS + TVN_BITS)) & TVN_MASK;
        vec = base->sltv3.vec + i;
    }
    else {
        // 64bit jiffies? 
        INIT_LIST_HEAD(&timer->list);
    }
    list_add(&timer->list, vec->prev);
}

EXPORT int kdtimer_add(kdtimer_t* timer)
{
    assert(timer && timer->base);
    if (timer->list.next)
        goto bug;
    internal_add_kdtimer(timer->base, timer);
    return 0;
bug:
    errno = EEXIST;
    return -1;
}

static inline int detach_kdtimer(kdtimer_t* timer)
{
    if (!kdtimer_pending(timer)) {
        errno = ENOENT; 
        return -1;
    }
    list_del(&timer->list);
    return 0;
}

EXPORT void kdtimer_mod(kdtimer_t* timer, unsigned long expires)
{
    int ret;
    timer->expires = expires;
    ret = detach_kdtimer(timer);
    internal_add_kdtimer(timer->base, timer);
}

EXPORT int kdtimer_del(kdtimer_t* timer)
{
    int ret;
    ret = detach_kdtimer(timer);
    timer->list.next = timer->list.prev = 0;
    return ret;
}

static inline void cascade_kdtimers(struct kdtimer_vec* tv)
{
    struct list_head *head, *curr, *next;

    head = tv->vec + tv->index;
    curr = head->next;

    while (curr != head) {
        kdtimer_t* tmp;
        tmp = list_entry(curr, kdtimer_t, list);
        next = curr->next;
        list_del(curr); 
        internal_add_kdtimer(tmp->base, tmp);
        curr = next;
    }
    INIT_LIST_HEAD(head);
    tv->index = (tv->index + 1) & TVN_MASK;
}

static inline void run_kdtimer_list(timer_base_t* base, unsigned long jiffies)
{
    while ((long) (jiffies - base->kdtimer_jiffies) >= 0) {
        struct list_head *head, *curr;
        if (!base->sltv1.index) {
            int n = 1;
            do {
                cascade_kdtimers(base->sltvecs[n]);
            } while (base->sltvecs[n]->index == 1 && ++n < NOOF_SLTVECS);
        }
repeat:
        head = base->sltv1.vec + base->sltv1.index;
        curr = head->next;
        if (curr != head) {
            kdtimer_t* timer;
            void (*fn)(void*);
            void* ctx;

            timer = list_entry(curr, kdtimer_t, list);
            fn = timer->function;
            ctx = timer->ctx;

            detach_kdtimer(timer);
            timer->list.next = timer->list.prev = NULL;
            fn(ctx);
            goto repeat;
        }
        base->kdtimer_jiffies += 1 << SHIFT_BITS;
        base->sltv1.index = (base->sltv1.index + 1) & TVR_MASK;
    }
}

EXPORT void kdtimer_collect()
{
    run_kdtimer_list(t_base, times(NULL));
}


EXPORT timer_base_t* kdtimer_init()
{
    timer_base_t* base = malloc(sizeof(timer_base_t));
    if (!base) return 0;
    init_kdtimer_vecs(base);
    t_base = base;
    return base;
}


EXPORT void kdtimer_cleanup(timer_base_t* base)
{
    if (!base) base = t_base;
    free(base);
}

EXPORT void kdtimer_set(kdtimer_t* timer, unsigned long expires, void (*fn)(void*), void* ctx)
{
	timer->list.next = timer->list.prev = NULL;
    timer->expires = times(NULL)+ expires/10;//0.01s
    timer->ctx = ctx;
    timer->function = fn;
    timer->base = t_base;
}

EXPORT void kdtimer_base_set(kdtimer_t* timer, timer_base_t* base)
{
    timer->base = base;
}

EXPORT void kdtimer_base_collect(timer_base_t* base)
{
    run_kdtimer_list(base, times(NULL));
}


