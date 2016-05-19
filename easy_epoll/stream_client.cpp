#include "stream_client.h"
#include "SocketApi.h"

stream_client::stream_client()
{
	m_pNetServer = NULL;
}

stream_client::~stream_client()
{
}

bool stream_client::Open(event_loop* pServer)
{
	if(pServer == NULL)
		return false;

	m_pNetServer = pServer;
	return true;
}

bool stream_client::Connect(tcpconn* pHandler, const std::string& strAddr, int port)
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

bool stream_client::Connect(tcpconn* pHandler, const NetAddr& addr)
{
	return Connect(pHandler,addr.host,addr.port);
}

bool stream_client::Register(tcpconn* pHandler)
{
	if(m_pNetServer == NULL)
		return false;
	return m_pNetServer->Register(pHandler);
}

