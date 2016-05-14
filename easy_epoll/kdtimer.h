#ifndef KDTIMER_HEADER
#define KDTIMER_HEADER

#define __KERNEL__
#include <stddef.h>
#include <sys/times.h>
#include "list.h"
#include "log.h"

typedef struct timer_base timer_base_t;

typedef struct kdtimer {
	struct list_head list;
	unsigned long expires;
	void* ctx;
	void (*function)(void*);
    timer_base_t* base;
} kdtimer_t;

static void kdtimer_set(kdtimer_t* timer, unsigned long expires, void (*fn)(void*), void* ctx);
static int  kdtimer_add(kdtimer_t* timer);
static int  kdtimer_del(kdtimer_t* timer);
static void kdtimer_mod(kdtimer_t* timer, unsigned long expires);

static timer_base_t* kdtimer_init();
static void kdtimer_collect();
static void kdtimer_cleanup(timer_base_t*);

static void kdtimer_base_set(kdtimer_t* timer, timer_base_t*);
static void kdtimer_base_collect(timer_base_t*);

static inline int kdtimer_valid(const kdtimer_t* timer)
{
	return timer->list.next != NULL;
}
#define kdtimer_pending kdtimer_valid 

#endif
