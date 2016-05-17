#ifndef TIMEROUTEVENT_HEADER
#define TIMEROUTEVENT_HEADER

class TimerOutEvent
{
public:
    TimerOutEvent(){}
    virtual ~TimerOutEvent(){}
	virtual int ProcessOnTimerOut(int Timerid)=0;
};


#endif 


