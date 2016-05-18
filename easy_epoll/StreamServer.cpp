#include "StreamServer.h"
#include "SocketApi.h"
#include <stdarg.h>
#include <assert.h>

StreamServer::StreamServer(void)
:event_loop()
{
	m_nMaxID = 0;
}

StreamServer::~StreamServer(void)
{
}

TcpHandler * StreamServer::CreateHandler(void)
{
	StreamHandler *pNewHandler = NULL;
    int nHandlerID = GetUseID();
	pNewHandler = new StreamHandler(nHandlerID);
	return pNewHandler;
}

void  StreamServer::OnConnect(StreamHandler *pHandler )
{
    int id = pHandler->GetHandlerID();
    if(m_HandlerMap.find(id) == m_HandlerMap.end())
    {
        m_HandlerMap.insert(std::map<int, StreamHandler*>::value_type(id,pHandler));
    }
    else
    {
        log_debug("StreamServer::ProcessConnected Error %d\r\n", pHandler->GetHandlerID());
        assert(false);
    }
}

void  StreamServer::OnDisconnect(StreamHandler *pHandler )
{
    int id = pHandler->GetHandlerID();
    std::map<int, StreamHandler*>::iterator iter = m_HandlerMap.find(id);
    if(iter != m_HandlerMap.end())
    {
        m_HandlerMap.erase(iter);
    }
    else
    {
        log_debug("StreamServer::ProcessClose Error %d\r\n",pHandler->GetHandlerID());
        assert(false);
    }
}

int StreamServer::ProcessOnTimer(StreamHandler *pHandler)
{
    log_debug("connect 30s and no packet,disconnect \n");
    DisConnect(pHandler);
	return 0;
}

StreamHandler * StreamServer::FindHandler(int nIndex)
{
	std::map<int, StreamHandler*>::iterator iter = m_HandlerMap.find(nIndex);

	if(iter != m_HandlerMap.end())
	{
		return iter->second;
	}
	return NULL;
}

int StreamServer::GetUseID(void)
{
    ++m_nMaxID;
    while(m_HandlerMap.find(m_nMaxID) != m_HandlerMap.end())
    {
        ++m_nMaxID;
    }
	return m_nMaxID;
}
