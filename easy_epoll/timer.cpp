#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <sys/times.h>
#include "timer.h"
#include "list.h"
#include "log.h"

#define SHIFT_BITS 6
#define TVN_BITS 8
#define TVR_BITS 10
#define TVN_SIZE (1 << TVN_BITS)//2^8
#define TVR_SIZE (1 << TVR_BITS)//1024
#define TVN_MASK (TVN_SIZE - 1)//2^8 - 1
#define TVR_MASK (TVR_SIZE - 1)//1023
#define NOOF_SLTVECS 3

typedef struct timer_base timer_base_t;

typedef struct kdtimer {
	struct list_head list;
	unsigned long expires;
	void* ctx;
	void (*function)(void*);
    timer_base_t* base;
} kdtimer_t;

struct kdtimer_vec {
    int index;
    struct list_head vec[TVN_SIZE];
};

struct kdtimer_vec_root {
    int index;
    struct list_head vec[TVR_SIZE];
};

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
    printf("expires:%lu(%ld), idx:%lu(%ld)\n",expires,expires,idx,idx);
    struct list_head* vec;
    if (idx < 1 << (SHIFT_BITS + TVR_BITS)) {
        printf("level 1\n");
        int i = (expires >> SHIFT_BITS) & TVR_MASK;
        vec = base->sltv1.vec + i;
    }
    else if (idx < 1 << (SHIFT_BITS + TVR_BITS + TVN_BITS)) {
        printf("level 2\n");
        int i = (expires >> (SHIFT_BITS + TVR_BITS)) & TVN_MASK;
        vec = base->sltv2.vec + i;
    }
    else if ((signed long)idx < 0) {
        // already expired
        printf("level -1\n");
        vec = base->sltv1.vec + base->sltv1.index;
    }
    else if (idx <= 0xffffffffUL) {
        printf("level 3\n");
        int i = (expires >> (SHIFT_BITS + TVR_BITS + TVN_BITS)) & TVN_MASK;
        vec = base->sltv3.vec + i;
    }
    else {
        printf("level nil\n");
        // 64bit jiffies? not for long interval timer
        INIT_LIST_HEAD(&timer->list);
    }
    list_add(&timer->list, vec->prev);
}

static inline timer_base_t* kdtimer_init()
{
    timer_base_t* base = (timer_base_t*)malloc(sizeof(timer_base_t));
    if (!base) return 0;
    init_kdtimer_vecs(base);
    t_base = base;
    return base;
}

static inline int kdtimer_add(kdtimer_t* timer)
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

static inline void kdtimer_set(kdtimer_t* timer, unsigned long expires, void (*fn)(void*), void* ctx)
{
	timer->list.next = timer->list.prev = NULL;
    timer->expires = times(NULL) + expires/10;//0.01s
    timer->ctx = ctx;
    timer->function = fn;
    timer->base = t_base;
}

static inline int kdtimer_pending(const kdtimer_t* timer)
{
	return timer->list.next != NULL;
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

static inline int kdtimer_del(kdtimer_t* timer)
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
    tv->index = (tv->index + 1) & TVN_MASK; // will rewind to 0
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
        base->sltv1.index = (base->sltv1.index + 1) & TVR_MASK; // will rewind to 0
    }
}

static inline void kdtimer_collect()
{
    run_kdtimer_list(t_base, times(NULL));
}

static void on_timer(void* ctx)
{
	struct time_ev* ev = (struct time_ev*)ctx;
	stop_timer(ev);	    
	ev->callback(ev->ptr);
}

void init_timer()
{
	kdtimer_init();
	run_timer();
}

int start_timer(int sec, int usec, struct time_ev* ev)
{
	void* timer = 0;

	//timer malloc
	timer = malloc(sizeof(kdtimer_t));

	//add timer
	kdtimer_set((kdtimer_t*)timer, sec*1000 + usec / 1000, &on_timer, ev);
	if (0 != kdtimer_add((kdtimer_t*)timer)) {
		free(timer);
		return -1;
	};
	ev->timer = timer;
	return 0;
}

int stop_timer(struct time_ev* ev)
{
	if (ev->timer) {
		kdtimer_del((kdtimer_t*)(ev->timer));
		free(ev->timer);
		ev->timer = NULL;
	}
	return 0;
}

void run_timer()
{
	kdtimer_collect();
}

static void handle_time_event(void* ptr)
{
    timer* t = (timer*)ptr;
    t->on_time_event(t->tev_.time_id);
}

timer::timer() :start_(false), interval_(0), timerid_(0), handler_(0) {
    tev_.time_id = 0;
    tev_.timer = 0;
    tev_.ptr = this;
    tev_.callback = handle_time_event;
}

timer::~timer() {
    stop();
}

void timer::set_timerid(int timer_id) {
    tev_.time_id = timer_id;
}

void timer::start(int sec, int usec) {
    if(start_) stop();
    if(sec <= 0) {
        log_error("invalid interval sec:%d", sec);
        return;
    }
    interval_ = sec;
    if(start_timer(sec, usec, &tev_) < 0) {
        log_error("start_timer failed\n");
        return;
    }
    start_ = true;
}

void timer::stop() {
    if (start_) stop_timer(&tev_);
}

void timer::reset() {
    stop();
    start(interval_);
}

void set_handler(timer_handler* h, int id) {
    timerid_ = id;
    handler_ = h;
}

void on_time_event(int timer_id) {
    start_ = false;
    if(handler_ != 0)    
        handler_->on_timeout(m_nId);
    else
        log_error("handler_ is null");  
}
