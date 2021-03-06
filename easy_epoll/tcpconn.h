#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

#include "timer.h"
#include "loopbuf.h"
#include "message_parser.h"
#include "sockapi.h"
#include "eventloop.h"
#include <stdint.h>
#include <map>
#include <string>

class inmessage;
class outmessage;

const int MAX_LOOP_BUFFER_LEN = 64 * 1024;

#define RECV_BUFFER_SIZE (1024 * 32)
#define SEND_BUFFER_SIZE (1024 * 32)

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

	eventloop* evloop() { return evloop_; }
	void evloop(eventloop* p) { evloop_ = p; }

	event_handler* evhandler() { return evhandler_; }
	void evhandler(event_handler* p) { evhandler_ = p; }

	const bool isconnecting() const { return status_ == CONNECTING; }
	void setconnecting() { status_ = CONNECTING; }
	const int status() const { return status_; }

	const int connid() const { return connid_; }

	const std::string& remoteaddr() const { return remoteaddr_; }
	void setremoteaddr();

	void* getud() { return ud_; }	
	void setud(void* ud) { ud_ = ud; }

	// used by eventloop
	int handle_accept();
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
	// init 0, server: set by stream_server::handle_accept, client: set by stream_client::connect
	int sockfd_; 

	// init 0, set by eventloop:addfd
    uint32_t fdidx_;

    // init false, server: set by stream_server::handle_accept, client: set by stream_client::stream_client
    bool needdel_;

    // init false, set true when sendloopbuf_ has not enough space for sendbuf
    bool full_;

    // use only one time, test situations that connect but no data received
	timer tcptimer_;

	// server: set by stream_server::handle_accept, client: set by stream_client::connect
	eventloop* evloop_;  // pass in from outside, tcpconn dont care its lifetime

	// server: set by stream_server::handle_accept, client: set by stream_client::connect
	event_handler* evhandler_; // pass in from outside, tcpconn dont care its lifetime

	// for sys read
	char recvbuf_[RECV_BUFFER_SIZE];

	// for app send
	loopbuf* sendloopbuf_;

	// for sys send
	char tmpsendbuf_[SEND_BUFFER_SIZE];

	// only client may set to CONNECTING 
	int status_;

	// set when tcpconn is constructing
	int connid_;

	// set by handle_connect
	std::string remoteaddr_;

	// set by app
	void* ud_;

	// init by process_rawdata
    message_parser* parser_;
};

#endif  


