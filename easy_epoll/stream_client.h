#ifndef STREAM_CLIENT_HEADER
#define STREAM_CLIENT_HEADER

#include "tcpconn.h"
#include "event_loop.h"
#include "SocketApi.h"
#include <string>

class stream_client 
{
public:
	stream_client();
	virtual ~stream_client();
	bool Open(event_loop* pServer);
	bool Connect(tcpconn* pHandler, const std::string& strAddr,int port);
	bool Connect(tcpconn* pHandler, const NetAddr& addr);
protected:
	bool Register(tcpconn* pHandler);
	event_loop* m_pNetServer;
};



#endif

