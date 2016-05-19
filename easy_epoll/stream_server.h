#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "event_loop.h"
#include "server_tcpconn.h"
#include "timer.h"
#include <map>

class stream_server:public event_loop
{
public:
	stream_server(void);
	virtual ~stream_server(void);

	virtual tcpconn* create_tcpconn(void);
    virtual void OnConnect(server_tcpconn *pHandler );
    virtual void OnDisconnect(server_tcpconn *pHandler );
	virtual int ProcessOnTimer(server_tcpconn *);
	virtual int ProcessMessage(inmessage *pPacket, server_tcpconn *pHandler, unsigned long dwSessionID) = 0;
	server_tcpconn* FindHandler(int nIndex);
	int GetUseID(void);

private:
    int m_nMaxID;
    std::map<int, server_tcpconn*> m_HandlerMap;
};

#endif

