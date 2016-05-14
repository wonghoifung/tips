#include <stdio.h>
#include "timer.h"
#include "llist.h"	

#include "kdtimer.c"
#include "plex.h"
plex_t timer_plex;

void init_timer()
{
	kdtimer_init();
	plex_init(&timer_plex, sizeof(kdtimer_t));
	run_timer();
}

void on_timer(void* ctx)
{
	struct time_ev* ev = ctx;
	stop_timer(ev);	    
	ev->callback(ev->ptr);
}

int start_timer(int sec, int usec, struct time_ev* ev)
{
	void* timer = 0;

	//timer malloc
	timer = plex_alloc(&timer_plex);
	if (timer == NULL) {
		if (0 != plex_expand(&timer_plex, 64, &sys_malloc, 0)) {
			return -1;
		}
		timer = plex_alloc(&timer_plex);
		assert(timer);
	}

	//add timer
	kdtimer_set(timer, sec*1000 + usec / 1000, &on_timer, ev);
	if (0 != kdtimer_add(timer)) {
		plex_free(&timer_plex, timer);
		return -1;
	};
	ev->timer = timer;
	return 0;
}

int stop_timer(struct time_ev* ev)
{
	if (ev->timer) {
		kdtimer_del(ev->timer);
		plex_free(&timer_plex, ev->timer);
		ev->timer = NULL;
	}
	return 0;
}

void run_timer()
{
	kdtimer_collect();
}


