#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

#define RECV_BUFFER 4096

#include "timer.h"
#include "loopbuf.h"
#include "message_parser.h"
#include "sockapi.h"
#include <stdint.h>
#include <map>
#include <string>

class event_loop;
class inmessage;
class outmessage;

const int MAX_LOOP_BUFFER_LEN = 64 * 1024;

#define RECV_BUFFER_SIZE (1024 * 32)
#define SEND_BUFFER_SIZE (1024 * 32)

class tcpconn;

class tcpconn : public timer_handler
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);

public:
	tcpconn(int cid);	
	virtual ~tcpconn();	

	const int getfd() const { return sockfd_; }
	void setfd(int fd) { sockfd_ = fd; }

    const uint32_t getfdidx() const { return fdidx_; }
    void setfdidx(uint32_t index) { fdidx_ = index; }
  
    const bool getneeddel() const { return needdel_; }
    void setneeddel(bool d) { needdel_ = d; }

	event_loop* evloop() { return evloop_; }
	void evloop(event_loop* p) { evloop_ = p; }

	const bool isconnecting() const { return status_ == CONNECTING; }
	void setconnecting() { status_ = CONNECTING; }
	const int status() const { return status_; }

	const int connid() const { return connid_; }

	const std::string& remoteaddr() const { return remoteaddr_; }
	void setremoteaddr();

	void* getud() { return ud_; }	
	void setud(void* ud) { ud_ = ud; }

	// used by event_loop
	int handle_connect();
	int handle_read();
	int handle_write();
	int handle_close();

	int sendbuf(const char* buf, int nLen);
	int sendmsg(outmessage* msg);
    bool writable();

	int process_message(inmessage*); // used by message_parser
	int process_rawdata(char* buf, int nLen); // used by handle_read()

	// from timer_handler
	virtual int	on_timeout(int timerid);
	
private:
	int sockfd_;
    uint32_t fdidx_;
    bool needdel_;
    bool full_;
	timer tcptimer_;
	event_loop* evloop_;
	char recvbuf_[RECV_BUFFER_SIZE];	
	loopbuf* sendloopbuf_;
	char tmpsendbuf_[SEND_BUFFER_SIZE];

	int status_;
	int connid_;
	std::string remoteaddr_;
	void* ud_;
    message_parser* parser_;
};

#endif  


