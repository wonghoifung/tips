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
	int sock_fd = sockapi::socket_create();
	if( conn == NULL || sock_fd < 0)
	{	
		return false;
	}
	if(sockapi::client_connect(sock_fd, strAddr.c_str(), port) == 0)
	{	
        sockapi::socket_buffer(sock_fd,16*1024);
        if(sockapi::socket_nonblock(sock_fd) < 0)
        {
            log_error("SetNonblock faild \n");
            sockapi::socket_close(sock_fd);
            return false;
        }
        if(sockapi::socket_keepalive(sock_fd) < 0)
        {
            log_error("socket_keepalive faild \n");
            sockapi::socket_close(sock_fd);
            return false;
        }
        conn->setfd(sock_fd);
		return Register(conn);			
	}
	sockapi::socket_close(sock_fd);	
	return false;
}

bool stream_client::Connect(tcpconn* conn, const address& addr)
{
	return Connect(conn,addr.host,addr.port);
}

bool stream_client::Register(tcpconn* conn)
{
	if(evloop_ == NULL)
		return false;
	return evloop_->manage(conn);
}

