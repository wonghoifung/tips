#ifndef STREAMSERVER_HEADER
#define STREAMSERVER_HEADER

#include "TcpServer.h"
#include "StreamHandler.h"
#include "timer.h"
#include <map>

class StreamServer:public TcpServer
{
public:
	StreamServer(void);
	virtual ~StreamServer(void);

	virtual TcpHandler* CreateHandler(void);
    virtual void OnConnect(StreamHandler *pHandler );
    virtual void OnDisconnect(StreamHandler *pHandler );
	virtual int ProcessOnTimer(StreamHandler *);
	virtual int ProcessMessage(InMessage *pPacket, StreamHandler *pHandler, unsigned long dwSessionID) = 0;
	StreamHandler* FindHandler(int nIndex);
	int GetUseID(void);

private:
    int m_nMaxID;
    std::map<int, StreamHandler*> m_HandlerMap;
};

#endif

