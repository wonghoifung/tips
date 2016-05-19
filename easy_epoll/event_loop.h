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
	bool init_event();
	bool run();
    
	virtual tcpconn* create_tcpconn(void) = 0;

	tcpconn* prepare_tcpconn(int sock_fd);
	bool manage(tcpconn* conn);
	bool disconnect(tcpconn* conn);

    void towrite(tcpconn* s);
    void toread(tcpconn* s);

protected:
	int handle_accept();
    void addsock(tcpconn* s);
    void remsock(tcpconn* s);

private:
    void handle_close(tcpconn* conn);

protected:
    static bool run_;
    int listen_sockfd_;
	int maxfd_;
    tcpconn** fdconns_;    
    int fdcount_;
    uint32_t fdidx_; 
	int epollfd_;
	struct epoll_event* epevarr_; 
};

#endif

