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
,m_nHandlerID(nID)
{	
	m_nStatus = -1;
	m_addrremote = "";
	m_nPort = 0;
	m_pUserData = NULL;
    m_pParser = NULL;
}

server_tcpconn::~server_tcpconn(void)
{
    if (m_pParser != NULL)
    {
        delete m_pParser;
        m_pParser = NULL;
    }
}

int server_tcpconn::sendmsg(outmessage *pPacket)
{
	return tcpconn::sendbuf(pPacket->cbuffer(), pPacket->size());
}

int server_tcpconn::on_rawdata(char *buf, int nLen)
{
	m_nStatus = REQUEST;
	tcptimer_.stop();	

    if(m_pParser == NULL)
        m_pParser = message_parser::create(this);

	return m_pParser->parse(buf, nLen);
}

int server_tcpconn::on_message(inmessage *pPacket)
{
	stream_server *pServer = (stream_server *)this->evloop();
	return pServer->ProcessMessage(pPacket, this, m_nHandlerID);
}

int server_tcpconn::on_close(void)
{
	m_nStatus = CLOSE;	
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->OnDisconnect(this);
    return 0;
}

int server_tcpconn::on_connect(void)
{
	m_nStatus = CONNECT;
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
		m_addrremote = inet_ntoa(remote_addr.sin_addr);
		m_nPort = ntohs(remote_addr.sin_port);
	}
}
