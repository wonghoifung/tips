#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "event_loop.h"
#include "server_tcpconn.h"
#include "timer.h"
#include <map>

class stream_server : public event_loop
{
public:
	stream_server();
	virtual ~stream_server();
	virtual tcpconn* create_tcpconn();
    virtual void handle_connect(server_tcpconn* conn);
    virtual void handle_disconnect(server_tcpconn* conn);
	virtual int handle_timeout(server_tcpconn*);
	virtual int handle_message(inmessage* msg, server_tcpconn* conn, unsigned long ssid) = 0;
	server_tcpconn* findconn(int idx);
	int genconnid();

private:
    int maxid_;
    std::map<int, server_tcpconn*> connmap_;
};

#endif

