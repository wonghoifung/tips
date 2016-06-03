#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

extern "C" {
#include "ae.h"
#include "anet.h"
}

class inmessage;

class tcpconn
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);
public:
	tcpconn(aeEventLoop* evloop, int fd);
	~tcpconn();
	const int fd() const { return fd_; }
	int process_message(inmessage* imsg); 
private:
	aeEventLoop* evloop_;
	int fd_;
};

#endif
