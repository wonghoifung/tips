#ifndef MUTESERVER_HEADER
#define MUTESERVER_HEADER

#include "stream_server.h"
#include "timer.h"

class MuteServer : public stream_server
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
