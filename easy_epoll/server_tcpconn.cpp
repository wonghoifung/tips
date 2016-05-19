#include "server_tcpconn.h"
#include "stream_server.h"
#include "sockapi.h"
#include <time.h>

namespace
{
    static const int s_DisNoMsgTime = 30;
}

server_tcpconn::server_tcpconn(int nID)
:tcpconn()
,connid_(nID)
{	
	status_ = -1;
	remoteaddr_ = "";
	port_ = 0;
	ud_ = NULL;
    parser_ = NULL;
}

server_tcpconn::~server_tcpconn(void)
{
    if (parser_ != NULL)
    {
        delete parser_;
        parser_ = NULL;
    }
}

int server_tcpconn::sendmsg(outmessage *pPacket)
{
	return tcpconn::sendbuf(pPacket->cbuffer(), pPacket->size());
}

int server_tcpconn::on_rawdata(char *buf, int nLen)
{
	status_ = REQUEST;
	tcptimer_.stop();	

    if(parser_ == NULL)
        parser_ = message_parser::create(this);

	return parser_->parse(buf, nLen);
}

int server_tcpconn::on_message(inmessage *pPacket)
{
	stream_server *pServer = (stream_server *)this->evloop();
	return pServer->ProcessMessage(pPacket, this, connid_);
}

int server_tcpconn::on_close(void)
{
	status_ = CLOSE;	
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->OnDisconnect(this);
    return 0;
}

int server_tcpconn::on_connect(void)
{
	status_ = CONNECT;
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->OnConnect(this);

	tcptimer_.start(s_DisNoMsgTime);
	GetRemoteAddr();
    return 0;
}

int	server_tcpconn::on_timeout(int Timerid)
{
    stream_server *pServer = (stream_server*)this->evloop();
    int nRet = pServer->ProcessOnTimer(this);
    return nRet;
}

void server_tcpconn::GetRemoteAddr(void)
{
	sockaddr_in remote_addr;
	memset(&remote_addr, 0, sizeof(remote_addr));
	int len = sizeof(remote_addr);
	if(getpeername(getfd(), reinterpret_cast<sockaddr *> (&remote_addr), (socklen_t*)&len) == 0)
	{
		remoteaddr_ = inet_ntoa(remote_addr.sin_addr);
		port_ = ntohs(remote_addr.sin_port);
	}
}
