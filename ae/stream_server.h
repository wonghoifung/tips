#ifndef STREAM_SERVER_HEADER
#define STREAM_SERVER_HEADER

#include "tcpconn.h"

class stream_server
{
	stream_server(const stream_server&);
	stream_server& operator=(const stream_server&);
	struct channel {
		stream_server* server;
		int fd;
		int index;
	};
public:
	stream_server(aeEventLoop* evloop);
	~stream_server();
	bool init(int maxccnt, int port);
private:
	aeEventLoop* evloop_;
	int listenfd_;
	int maxconncount_;
	channel* channels_;
	tcpconn** tcpconns_;
};

#endif
