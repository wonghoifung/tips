#include "stream_server.h"
#include "sockapi.h"
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

tcpconn * stream_server::create_tcpconn(void)
{
	server_tcpconn* conn = NULL;
    int uid = GetUseID();
	conn = new server_tcpconn(uid);
	return conn;
}

void  stream_server::OnConnect(server_tcpconn *pHandler )
{
    int id = pHandler->GetHandlerID();
    if(m_HandlerMap.find(id) == m_HandlerMap.end())
    {
        m_HandlerMap.insert(std::map<int, server_tcpconn*>::value_type(id,pHandler));
    }
    else
    {
        log_debug("stream_server::ProcessConnected Error %d\r\n", pHandler->GetHandlerID());
        assert(false);
    }
}

void  stream_server::OnDisconnect(server_tcpconn *pHandler )
{
    int id = pHandler->GetHandlerID();
    std::map<int, server_tcpconn*>::iterator iter = m_HandlerMap.find(id);
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

int stream_server::ProcessOnTimer(server_tcpconn *pHandler)
{
    log_debug("connect 30s and no packet,disconnect \n");
    DisConnect(pHandler);
	return 0;
}

server_tcpconn * stream_server::FindHandler(int nIndex)
{
	std::map<int, server_tcpconn*>::iterator iter = m_HandlerMap.find(nIndex);

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
