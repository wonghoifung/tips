#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "event_loop.h"
#include "tcpconn.h"
#include "timer.h"
#include <map>

class stream_server : public event_handler
{
public:
	stream_server();
	virtual ~stream_server();
	virtual tcpconn* create_tcpconn();
    virtual void handle_connect_event(tcpconn* conn);
    virtual void handle_disconnect_event(tcpconn* conn);
	virtual int handle_timeout_event(tcpconn*);
	virtual int handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid);

	// TODO on_overload
	virtual void on_connect(tcpconn* conn) = 0;
    virtual void on_disconnect(tcpconn* conn) = 0;
	virtual int on_no_message(tcpconn* conn) = 0;
	virtual int on_message(inmessage* msg, tcpconn* conn, unsigned long ssid) = 0;

	tcpconn* findconn(int idx);
	int genconnid();

private:
    int maxid_;
    std::map<int, tcpconn*> connmap_;
};

#endif

