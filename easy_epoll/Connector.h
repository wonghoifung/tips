#ifndef CONNECTOR_HEADER
#define CONNECTOR_HEADER

#include "tcpconn.h"
#include "event_loop.h"
#include "SocketApi.h"
#include <string>

class Connector 
{
public:
	Connector();
	virtual ~Connector();
	bool Open(event_loop* pServer);
	bool Connect(tcpconn* pHandler, const std::string& strAddr,int port);
	bool Connect(tcpconn* pHandler, const NetAddr& addr);
protected:
	bool Register(tcpconn* pHandler);
	event_loop* m_pNetServer;
};



#endif

