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
	bool Open(event_loop* pServer);
	bool Connect(tcpconn* conn, const std::string& strAddr,int port);
	bool Connect(tcpconn* conn, const NetAddr& addr);
protected:
	bool Register(tcpconn* conn);
	event_loop* evloop_;
};



#endif

