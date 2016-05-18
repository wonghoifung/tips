#include "stream_server.h"
#include "SocketApi.h"
#include <stdarg.h>
#include <assert.h>

stream_server::stream_server(void)
:event_loop()
{
	m_nMaxID = 0;
}

stream_server::~stream_server(void)
{
}

tcpconn * stream_server::CreateHandler(void)
{
	StreamHandler *pNewHandler = NULL;
    int nHandlerID = GetUseID();
	pNewHandler = new StreamHandler(nHandlerID);
	return pNewHandler;
}

void  stream_server::OnConnect(StreamHandler *pHandler )
{
    int id = pHandler->GetHandlerID();
    if(m_HandlerMap.find(id) == m_HandlerMap.end())
    {
        m_HandlerMap.insert(std::map<int, StreamHandler*>::value_type(id,pHandler));
    }
    else
    {
        log_debug("stream_server::ProcessConnected Error %d\r\n", pHandler->GetHandlerID());
        assert(false);
    }
}

void  stream_server::OnDisconnect(StreamHandler *pHandler )
{
    int id = pHandler->GetHandlerID();
    std::map<int, StreamHandler*>::iterator iter = m_HandlerMap.find(id);
    if(iter != m_HandlerMap.end())
    {
        m_HandlerMap.erase(iter);
    }
    else
    {
        log_debug("stream_server::ProcessClose Error %d\r\n",pHandler->GetHandlerID());
        assert(false);
    }
}

int stream_server::ProcessOnTimer(StreamHandler *pHandler)
{
    log_debug("connect 30s and no packet,disconnect \n");
    DisConnect(pHandler);
	return 0;
}

StreamHandler * stream_server::FindHandler(int nIndex)
{
	std::map<int, StreamHandler*>::iterator iter = m_HandlerMap.find(nIndex);

	if(iter != m_HandlerMap.end())
	{
		return iter->second;
	}
	return NULL;
}

int stream_server::GetUseID(void)
{
    ++m_nMaxID;
    while(m_HandlerMap.find(m_nMaxID) != m_HandlerMap.end())
    {
        ++m_nMaxID;
    }
	return m_nMaxID;
}
