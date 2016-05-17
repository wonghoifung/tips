#ifndef TCPSERVER_HEADER
#define TCPSERVER_HEADER

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

#include "TcpHandler.h"

class TcpServer
{
public:
	TcpServer();
	virtual ~TcpServer();

    static void SigHandle(int signum);

	bool InitSocket(int listen_port);	
	bool Run();
    
	virtual TcpHandler* CreateHandler(void) = 0;

	TcpHandler* AllocSocketHandler(int sock_fd);
	bool Register(TcpHandler* pHandler);
	bool DisConnect(TcpHandler* pSocketHandler);

    void WantWrite(TcpHandler* s);
    void WantRead(TcpHandler* s);

protected:
	int handle_accept();
    void AddSocket(TcpHandler* s);
    void RemoveSocket(TcpHandler* s);

private:
	bool InitEvent();
    void handle_close(TcpHandler* pHandler);

protected:
    static bool m_bRun;
    int m_listen_fd;
	int m_maxfd;
    TcpHandler** fds;    
    int m_count_fd;
    uint32_t m_fd_index; 
	int m_epoll_fd;
	struct epoll_event* m_epev_arr; 
};

#endif

