#ifndef MUTESERVER_HEADER
#define MUTESERVER_HEADER

#include "StreamServer.h"
#include "TimerEvent.h"
#include "TimerOutEvent.h"

class MuteServer : public StreamServer
{
	MuteServer(const MuteServer&);
	MuteServer& operator=(const MuteServer&);
public:
	MuteServer();
	~MuteServer();
	bool init();
	virtual int ProcessMessage(InMessage* pMessage, StreamHandler* pHandler, unsigned long dwSessionID);
    virtual void OnConnect(StreamHandler* pHandler);
    virtual void OnDisconnect(StreamHandler* pHandler);
};

#endif
