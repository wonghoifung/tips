#include "Connector.h"
#include "SocketApi.h"

Connector::Connector()
{
	m_pNetServer = NULL;
}

Connector::~Connector()
{
}

bool Connector::Open(event_loop* pServer)
{
	if(pServer == NULL)
		return false;

	m_pNetServer = pServer;
	return true;
}

bool Connector::Connect(TcpHandler* pHandler, const std::string& strAddr, int port)
{
	int sock_fd = SocketApi::SocketInit();
	if( pHandler == NULL || sock_fd < 0)
	{	
		return false;
	}
	if(SocketApi::ClientConnect(sock_fd, strAddr.c_str(), port) == 0)
	{	
        SocketApi::SetSocketMem(sock_fd,16*1024);
        if(SocketApi::SocketNoBlock(sock_fd) < 0)
        {
            log_error("SetNonblock faild \n");
            SocketApi::SocketClose(sock_fd);
            return false;
        }
        if(SocketApi::SetTcpKeepLive(sock_fd) < 0)
        {
            log_error("SetTcpKeepLive faild \n");
            SocketApi::SocketClose(sock_fd);
            return false;
        }
        pHandler->SetFd(sock_fd);
		return Register(pHandler);			
	}
	SocketApi::SocketClose(sock_fd);	
	return false;
}

bool Connector::Connect(TcpHandler* pHandler, const NetAddr& addr)
{
	return Connect(pHandler,addr.host,addr.port);
}

bool Connector::Register(TcpHandler* pHandler)
{
	if(m_pNetServer == NULL)
		return false;
	return m_pNetServer->Register(pHandler);
}

