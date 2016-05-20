#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

#define RECV_BUFFER 4096

#include "timer.h"
#include "loopbuf.h"
#include "message_parser.h"
#include <stdint.h>
#include <map>
#include <string>

class event_loop;
class inmessage;
class outmessage;

const int MAX_LOOP_BUFFER_LEN = 64*1024;

#define RECV_BUFFER_SIZE (1024*32)
#define SEND_BUFFER_SIZE (1024*32)

// typedef int (*message_callback)(inmessage*);
// typedef int (*connect_callback)();
// typedef int (*close_callback)();
// typedef int (*rawdata_callback)();

class tcpconn : public timer_handler
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);

public:
	tcpconn(int cid);	
	~tcpconn();	

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
    bool writable();
	
	//-------------------------
	const int status(void) const { return status_; }
	const int connid(void) const { return connid_; }
	const std::string& remoteaddr(void) const { return remoteaddr_; }		
	void* getud() { return ud_; }	
	void setud(void* ud) { ud_ = ud; }
	int sendmsg(outmessage* msg);
	void setremoteaddr(void);
	virtual int on_message(inmessage*);
	virtual int on_rawdata(char* buf, int nLen);
	virtual int on_close(void);
	virtual int on_connect(void);
	virtual int	on_timeout(int timerid); // TODO
	
private:
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

	//-------------------------
	int status_;
	int connid_;
	std::string remoteaddr_;
	int port_;
	void* ud_;
    message_parser* parser_;

    // message_callback on_message_;
    // close_callback on_close_;
    // connect_callback on_connect_;
    // rawdata_callback on_rawdata_;
};

#endif  


