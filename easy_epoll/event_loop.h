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
public:
	event_loop();
	virtual ~event_loop();

    static void SigHandle(int signum);

	bool InitSocket(int listen_port);	
	bool InitEvent();
	bool Run();
    
	virtual tcpconn* CreateHandler(void) = 0;

	tcpconn* AllocSocketHandler(int sock_fd);
	bool Register(tcpconn* pHandler);
	bool DisConnect(tcpconn* pSocketHandler);

    void WantWrite(tcpconn* s);
    void WantRead(tcpconn* s);

protected:
	int handle_accept();
    void AddSocket(tcpconn* s);
    void RemoveSocket(tcpconn* s);

private:
	
    void handle_close(tcpconn* pHandler);

protected:
    static bool m_bRun;
    int m_listen_fd;
	int m_maxfd;
    tcpconn** fds;    
    int m_count_fd;
    uint32_t m_fd_index; 
	int m_epoll_fd;
	struct epoll_event* m_epev_arr; 
};

#endif

