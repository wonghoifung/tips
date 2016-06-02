#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "ae.h"

#define MAX_DESCRIPTORS 100000

class tcpconn;

class stream_server
{
	stream_server(const stream_server&);
	stream_server& operator=(const stream_server&);
public:
	stream_server(aeEventLoop* evloop);
	~stream_server();
	void start(int port);
	void handle_accept(int mask);
	tcpconn* create_tcpconn(int fd);
	void add_tcpconn(tcpconn* conn);
	void del_tcpconn(tcpconn* conn);
private:
	aeEventLoop* evloop_;
    int listenfd_;
    tcpconn** fdconns_;
    int fdcount_;
    uint32_t fdidx_;
};

#endif
