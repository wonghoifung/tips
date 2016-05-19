#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "event_loop.h"
#include "StreamHandler.h"
#include "timer.h"
#include <map>

class stream_server:public event_loop
{
public:
	stream_server(void);
	virtual ~stream_server(void);

	virtual tcpconn* CreateHandler(void);
    virtual void OnConnect(StreamHandler *pHandler );
    virtual void OnDisconnect(StreamHandler *pHandler );
	virtual int ProcessOnTimer(StreamHandler *);
	virtual int ProcessMessage(inmessage *pPacket, StreamHandler *pHandler, unsigned long dwSessionID) = 0;
	StreamHandler* FindHandler(int nIndex);
	int GetUseID(void);

private:
    int m_nMaxID;
    std::map<int, StreamHandler*> m_HandlerMap;
};

#endif

