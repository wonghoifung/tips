#ifndef TIMER_HEADER
#define TIMER_HEADER

#ifdef __cplusplus
extern "C" {
#endif

struct time_ev
{
	int time_id;
	void* timer;
	void* ptr;
	void (*callback)(void* ptr);
};

void init_timer(void);

int  start_timer(int sec, int usec, struct time_ev* ev);

int  stop_timer(struct time_ev* ev);

void run_timer(void);

#ifdef __cplusplus
}
#endif

#endif
