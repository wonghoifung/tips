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
	virtual int handle_message(inmessage* pMessage, tcpconn* conn, unsigned long dwSessionID);
    virtual void OnConnect(tcpconn* conn);
    virtual void OnDisconnect(tcpconn* conn);
};

#endif
