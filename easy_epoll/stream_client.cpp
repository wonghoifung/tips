#include "stream_client.h"
#include "sockapi.h"

stream_client::stream_client()
{
	evloop_ = NULL;
}

stream_client::~stream_client()
{
}

bool stream_client::Open(event_loop* evloop)
{
	if(evloop == NULL)
		return false;

	evloop_ = evloop;
	return true;
}

bool stream_client::Connect(tcpconn* conn, const std::string& strAddr, int port)
{
	int sock_fd = sockapi::SocketInit();
	if( conn == NULL || sock_fd < 0)
	{	
		return false;
	}
	if(sockapi::ClientConnect(sock_fd, strAddr.c_str(), port) == 0)
	{	
        sockapi::SetSocketMem(sock_fd,16*1024);
        if(sockapi::SocketNoBlock(sock_fd) < 0)
        {
            log_error("SetNonblock faild \n");
            sockapi::SocketClose(sock_fd);
            return false;
        }
        if(sockapi::SetTcpKeepLive(sock_fd) < 0)
        {
            log_error("SetTcpKeepLive faild \n");
            sockapi::SocketClose(sock_fd);
            return false;
        }
        conn->setfd(sock_fd);
		return Register(conn);			
	}
	sockapi::SocketClose(sock_fd);	
	return false;
}

bool stream_client::Connect(tcpconn* conn, const NetAddr& addr)
{
	return Connect(conn,addr.host,addr.port);
}

bool stream_client::Register(tcpconn* conn)
{
	if(evloop_ == NULL)
		return false;
	return evloop_->manage(conn);
}

