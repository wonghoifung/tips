#ifndef TIMEREVENT_HEADER
#define TIMEREVENT_HEADER

#include "timer.h"
#include "TimerOutEvent.h"

class TimerEvent
{
public:
	TimerEvent(void);
	virtual ~TimerEvent(void);
	void SetTimerId(int timer_id);
	void StartTimer(int sec, int usec = 0);
	void StopTimer();
	void ResetTimer();	
	void SetTimeEventObj(TimerOutEvent* obj, int id=0);
	virtual void OnTimer(int timer_id);

	time_ev m_ev;

protected:
	bool m_bHaveStart;
	int m_timeout;
	int m_nId;
	TimerOutEvent* m_TimeEvent;
};

#endif
