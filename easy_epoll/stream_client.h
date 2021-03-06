#ifndef STREAMCLIENT_HEADER
#define STREAMCLIENT_HEADER

#include "eventloop.h"
#include "sockapi.h"

class outmessage;

class stream_client : public event_handler
{
	stream_client(const stream_client&);
	stream_client& operator=(const stream_client&);

public:
	stream_client(eventloop* evloop);
	virtual ~stream_client();

	tcpconn* create_tcpconn();

	bool connect(const std::string& host, const std::string& port);
	bool connect(const std::string& host, int port);
 	int send(outmessage* msg);

 	// from event_handler
    virtual void handle_connect_event(tcpconn* conn);
    virtual void handle_disconnect_event(tcpconn* conn);
	virtual int handle_timeout_event(tcpconn*);
	virtual int handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid);

	// to subclass
	virtual void on_connect(tcpconn* conn) = 0;
    virtual void on_disconnect(tcpconn* conn) = 0;
	virtual int on_no_message(tcpconn* conn) = 0;
	virtual int on_message(inmessage* msg, tcpconn* conn, unsigned long ssid) = 0;

protected:
	eventloop* evloop_; // pass in from outside, stream_client dont care its lifetime
	tcpconn* conn_;
};

#endif
