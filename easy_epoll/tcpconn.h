#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

#define RECV_BUFFER 4096

#include "timer.h"
#include "loopbuf.h"
#include <stdint.h>
#include <map>

class event_loop;
class inmessage;

const int MAX_LOOP_BUFFER_LEN = 64*1024;

#define RECV_BUFFER_SIZE (1024*32)
#define SEND_BUFFER_SIZE (1024*32)

class tcpconn : public timer_handler
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);

public:
	tcpconn();	
	virtual ~tcpconn();	

	const int getfd() const { return sockfd_; }
	void setfd(int fd) { sockfd_ = fd; }

    const uint32_t getfdidx() const { return fdidx_; }
    void setfdidx(uint32_t index) { fdidx_ = index; }
  
    const bool getneeddel() const { return needdel_; }
    void setneeddel(bool d) { needdel_ = d; }

	event_loop* evloop() { return evloop_; }
	void evloop(event_loop* p) { evloop_ = p; }

	int handle_connect();
	int handle_read();
	int handle_write();
	int handle_close();

	int sendbuf(const char* buf, int nLen);
    virtual int on_message(inmessage*) = 0;
    bool writable();
	
protected:
	virtual int on_close(void) { return -1; }
	virtual int on_connect(void) { return 0; }
	virtual int on_rawdata(char*, int) { return -1; }
	virtual int	on_timeout(int timerid) { return 0; }
	
	int sockfd_;
    uint32_t fdidx_;             
	uint32_t socktype_;
    bool needdel_;
    bool full_;
	timer tcptimer_;
	event_loop* evloop_;
	char recvbuf_[RECV_BUFFER_SIZE];	
	loopbuf* sendloopbuf_;
	char tmpsendbuf_[SEND_BUFFER_SIZE];
};

#endif  


