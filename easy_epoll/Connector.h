#ifndef CONNECTOR_HEADER
#define CONNECTOR_HEADER

#include "TcpHandler.h"
#include "event_loop.h"
#include "SocketApi.h"
#include <string>

class Connector 
{
public:
	Connector();
	virtual ~Connector();
	bool Open(event_loop* pServer);
	bool Connect(TcpHandler* pHandler, const std::string& strAddr,int port);
	bool Connect(TcpHandler* pHandler, const NetAddr& addr);
protected:
	bool Register(TcpHandler* pHandler);
	event_loop* m_pNetServer;
};



#endif

