#include <stdio.h>
#include "TimerEvent.h"
#include "log.h"
#include <time.h>

void handle_timeout(void* ptr)
{
	TimerEvent* sh = (TimerEvent*)ptr;
	sh->OnTimer(sh->m_ev.time_id);
}

TimerEvent::TimerEvent(void)
:m_bHaveStart(false)
,m_timeout(0)
,m_nId(0)
,m_TimeEvent(0)
{
	m_ev.time_id = 0;
	m_ev.timer = 0;
	m_ev.ptr = this;
	m_ev.callback = handle_timeout;
}

TimerEvent::~TimerEvent(void)
{
	StopTimer();
}

void TimerEvent::StartTimer(int sec, int usec)
{
	if(m_bHaveStart)
		StopTimer();
    if(sec <= 0)
    {
        log_error("the timer is error %d ",sec);
        return;
    }

	m_timeout = sec;
	if(start_timer(sec, usec, &m_ev) < 0)
    {
        log_error("start_timer fail \n");
        return;
    }
	m_bHaveStart = true;
}

void TimerEvent::StopTimer()
{
	if(m_bHaveStart)
		stop_timer(&m_ev);
}

void TimerEvent::ResetTimer()
{
	StopTimer();
	StartTimer(m_timeout);
}

void TimerEvent::SetTimerId(int timer_id)
{
	m_ev.time_id = timer_id;
}

void TimerEvent::SetTimeEventObj(TimerOutEvent * obj, int id)
{
	m_nId = id;
	m_TimeEvent = obj;
}

void TimerEvent::OnTimer(int timer_id)
{
	m_bHaveStart = false;

	if(m_TimeEvent != 0)	
		m_TimeEvent->ProcessOnTimerOut(m_nId);
    else
        log_debug("Timeobj is null \n");  
}

