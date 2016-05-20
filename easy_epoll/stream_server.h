#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "event_loop.h"
#include "tcpconn.h"
#include "timer.h"
#include <map>

class stream_server : public event_loop
{
public:
	stream_server();
	virtual ~stream_server();
	virtual tcpconn* create_tcpconn();
    virtual void handle_connect(tcpconn* conn);
    virtual void handle_disconnect(tcpconn* conn);
	virtual int handle_timeout(tcpconn*);
	virtual int handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid) = 0;
	tcpconn* findconn(int idx);
	int genconnid();

private:
    int maxid_;
    std::map<int, tcpconn*> connmap_;
};

#endif

