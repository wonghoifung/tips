#ifndef CONNECTOR_HEADER
#define CONNECTOR_HEADER

#include "TcpHandler.h"
#include "TcpServer.h"
#include "SocketApi.h"
#include <string>

class Connector 
{
public:
	Connector();
	virtual ~Connector();
	bool Open(TcpServer* pServer);
	bool Connect(TcpHandler* pHandler, const std::string& strAddr,int port);
	bool Connect(TcpHandler* pHandler, const NetAddr& addr);
protected:
	bool Register(TcpHandler* pHandler);
	TcpServer* m_pNetServer;
};



#endif

