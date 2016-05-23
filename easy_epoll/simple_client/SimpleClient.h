#ifndef SIMPLECLIENT_HEADER
#define SIMPLECLIENT_HEADER

#include "stream_client.h"
#include "tcpconn.h"
#include "message.h"

class SimpleClient : public stream_client
{
	SimpleClient(const SimpleClient&);
	SimpleClient& operator=(const SimpleClient&);
public:
	SimpleClient();
	~SimpleClient();
	virtual void handle_connect(tcpconn* conn);
    virtual void handle_disconnect(tcpconn* conn);
	virtual int handle_timeout(tcpconn*);
	virtual int handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid);
};
#endif
