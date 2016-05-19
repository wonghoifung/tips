#include "stream_server.h"
#include "sockapi.h"
#include <stdarg.h>
#include <assert.h>

stream_server::stream_server()
:event_loop()
{
	maxid_ = 0;
}

stream_server::~stream_server()
{
}

tcpconn * stream_server::create_tcpconn()
{
	server_tcpconn* conn = NULL;
    int uid = genconnid();
	conn = new server_tcpconn(uid);
	return conn;
}

void  stream_server::handle_connect(server_tcpconn* conn)
{
    int id = conn->connid();
    if(connmap_.find(id) == connmap_.end())
    {
        connmap_.insert(std::map<int, server_tcpconn*>::value_type(id,conn));
    }
    else
    {
        log_debug("stream_server::ProcessConnected Error %d\r\n", conn->connid());
        assert(false);
    }
}

void  stream_server::handle_disconnect(server_tcpconn* conn)
{
    int id = conn->connid();
    std::map<int, server_tcpconn*>::iterator iter = connmap_.find(id);
    if(iter != connmap_.end())
    {
        connmap_.erase(iter);
    }
    else
    {
        log_debug("stream_server::ProcessClose Error %d\r\n",conn->connid());
        assert(false);
    }
}

int stream_server::handle_timeout(server_tcpconn* conn)
{
    log_debug("connect 30s and no packet,disconnect \n");
    disconnect(conn);
	return 0;
}

server_tcpconn* stream_server::findconn(int nIndex)
{
	std::map<int, server_tcpconn*>::iterator iter = connmap_.find(nIndex);

	if(iter != connmap_.end())
	{
		return iter->second;
	}
	return NULL;
}

int stream_server::genconnid()
{
    ++maxid_;
    while (connmap_.find(maxid_) != connmap_.end())
    {
        ++maxid_;
    }
	return maxid_;
}
