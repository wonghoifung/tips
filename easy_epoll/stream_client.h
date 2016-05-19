#ifndef STREAM_CLIENT_HEADER
#define STREAM_CLIENT_HEADER

#include "tcpconn.h"
#include "event_loop.h"
#include "sockapi.h"
#include <string>

class stream_client 
{
public:
	stream_client();
	virtual ~stream_client();
	bool set_evloop(event_loop* pServer);
	bool connect(tcpconn* conn, const std::string& strAddr, int port);
	bool connect(tcpconn* conn, const address& addr);
protected:
	bool Register(tcpconn* conn);
	event_loop* evloop_;
};



#endif

