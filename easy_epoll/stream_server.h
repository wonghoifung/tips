#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "eventloop.h"
#include "tcpconn.h"
#include "timer.h"
#include <map>

class stream_server : public event_handler
{
	stream_server(const stream_server&);
	stream_server& operator=(const stream_server&);
	
public:
	stream_server(eventloop* evloop);
	virtual ~stream_server();

	bool init(int listenport);
	int handle_accept();

	tcpconn* create_tcpconn();

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

	tcpconn* findconn(int idx);
	int genconnid();

	const int listenfd() const { return listenfd_; }

	eventloop* evloop() { return evloop_; }
	
private:
	eventloop* evloop_; // pass in from outside, stream_client dont care its lifetime
	int listenfd_;
    int maxid_;
    std::map<int, tcpconn*> connmap_;
};

#endif

