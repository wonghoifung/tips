#include "StreamHandler.h"
#include "stream_server.h"
#include "SocketApi.h"
#include <time.h>

namespace
{
    static const int s_DisNoMsgTime = 30;
}

StreamHandler::StreamHandler(int nID)
:tcpconn()
,m_nHandlerID(nID)
{	
	m_nStatus = -1;
	m_addrremote = "";
	m_nPort = 0;
	m_pUserData = NULL;
    m_pParser = NULL;
}

StreamHandler::~StreamHandler(void)
{
    if (m_pParser != NULL)
    {
        delete m_pParser;
        m_pParser = NULL;
    }
}

int StreamHandler::Send(OutMessage *pPacket)
{
	return tcpconn::Send(pPacket->cbuffer(), pPacket->size());
}

int StreamHandler::OnParser(char *buf, int nLen)
{
	m_nStatus = REQUEST;
	m_TcpTimer.stop();	

    if(m_pParser == NULL)
        m_pParser = MessageParser::create(this);

	return m_pParser->parse(buf, nLen);
}

int StreamHandler::OnParserComplete(InMessage *pPacket)
{
	stream_server *pServer = (stream_server *)this->evloop();
	return pServer->ProcessMessage(pPacket, this, m_nHandlerID);
}

int StreamHandler::OnClose(void)
{
	m_nStatus = CLOSE;	
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->OnDisconnect(this);
    return 0;
}

int StreamHandler::OnConnected(void)
{
	m_nStatus = CONNECT;
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->OnConnect(this);

	m_TcpTimer.start(s_DisNoMsgTime);
	GetRemoteAddr();
    return 0;
}

int	StreamHandler::on_timeout(int Timerid)
{
    stream_server *pServer = (stream_server*)this->evloop();
    int nRet = pServer->ProcessOnTimer(this);
    return nRet;
}

void StreamHandler::GetRemoteAddr(void)
{
	sockaddr_in remote_addr;
	memset(&remote_addr, 0, sizeof(remote_addr));
	int len = sizeof(remote_addr);
	if(getpeername(GetFd(), reinterpret_cast<sockaddr *> (&remote_addr), (socklen_t*)&len) == 0)
	{
		m_addrremote = inet_ntoa(remote_addr.sin_addr);
		m_nPort = ntohs(remote_addr.sin_port);
	}
}
