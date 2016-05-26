#ifndef TIMER_HEADER
#define TIMER_HEADER
// TODO new timer design
struct time_ev
{
	int time_id;
	void* timer;
	void* ptr;
	void (*callback)(void* ptr);
};

void init_timer(void);
void run_timer(void);

class timer_handler
{
public:
    timer_handler() {}
    virtual ~timer_handler() {}
	virtual int on_timeout(int Timerid) = 0;
};

class timer
{
	timer(const timer&);
	timer& operator=(const timer&);

public:
	timer();
	~timer();
	void set_timerid(int timer_id);
	void start(int sec, int usec = 0);
	void stop();
	void reset();
	void set_handler(timer_handler* h, int id = 0);
	void on_time_event(int timer_id);
	time_ev tev_;

private:
	bool start_;
	int interval_;
	int timerid_;
	timer_handler* handler_;
};

#endif
