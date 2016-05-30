#ifndef EVENT_LOOP_HEADER
#define EVENT_LOOP_HEADER

#include <sys/resource.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <map>
#include <vector>
#include "tcpconn.h"

class event_loop
{
	event_loop(const event_loop&);
	event_loop& operator=(const event_loop&);

public:
	event_loop();
	virtual ~event_loop();

    static void handle_signal(int signum);

	bool init_server(int listen_port);	
	bool init_client(const std::string& host, int port);
	bool init_event();
	bool run();
    
	virtual tcpconn* create_tcpconn(void) = 0;
	tcpconn* prepare_tcpconn(int sock_fd);
	bool disconnect(tcpconn* conn);

    void towrite(tcpconn* s);
    void toread(tcpconn* s);

protected:
	int handle_accept();
    void addsock(tcpconn* s); // call when tcpconn is setup ok 
    void remsock(tcpconn* s);
    void handle_close(tcpconn* conn);

    static bool run_; // set false when signal received, default true
    int listen_sockfd_; // server: init by init_server(), client: no use
    tcpconn** fdconns_; // init by init_event()
    int fdcount_; // increase 1 by addsock(), decrease 1 by remsock()
    uint32_t fdidx_; // generate idx for every new connection, set to tcpconn in addsock()
	int epollfd_; // init by init_event()
	struct epoll_event* epevarr_; // active events...init by init_event()
};

class event_handler : public event_loop
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

