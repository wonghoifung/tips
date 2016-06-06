#ifndef EVENTLOOP_HEADER
#define EVENTLOOP_HEADER

#include <sys/epoll.h>
#include <stdint.h>

class tcpconn;
class stream_server;
class inmessage;

class eventloop
{
	eventloop(const eventloop&);
	eventloop& operator=(const eventloop&);

public:
	eventloop();
	~eventloop();
	bool init(int maxfdcnt);
	void run();
	void toread(tcpconn* tc);
	void towrite(tcpconn* tc);
	void addlistenfd(stream_server* server);
	void dellistenfd(stream_server* server);
	void addfd(tcpconn* tc);
	void delfd(tcpconn* tc);
	void close_conn(tcpconn* conn);

	int stop;
	int epollfd;
	struct epoll_event* firedevents;
	int maxfdcount;
	int fdcount;
	uint32_t fdidx;
	tcpconn** fdconns;
	int listenfd; // pass in from outside, eventloop dont care its lifetime
};

// interface used by tcpconn, put it here just for convenience
class event_handler
{
	event_handler(const event_handler&);
	event_handler& operator=(const event_handler&);
	
public:
	event_handler() {}
	virtual ~event_handler() {}

    virtual void handle_connect_event(tcpconn* conn) = 0;
    virtual void handle_disconnect_event(tcpconn* conn) = 0;
	virtual int handle_timeout_event(tcpconn*) = 0;
	virtual int handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid) = 0;
};

#endif
