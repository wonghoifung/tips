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
	virtual void on_connect(tcpconn* conn);
    virtual void on_disconnect(tcpconn* conn);
	virtual int on_no_message(tcpconn*);
	virtual int on_message(inmessage* msg, tcpconn* conn, unsigned long ssid);
};
#endif
