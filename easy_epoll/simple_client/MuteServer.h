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
	virtual int ProcessMessage(inmessage* pMessage, server_tcpconn* pHandler, unsigned long dwSessionID);
    virtual void OnConnect(server_tcpconn* pHandler);
    virtual void OnDisconnect(server_tcpconn* pHandler);
};

#endif
