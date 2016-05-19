#include "sockapi.h"
#include "event_loop.h"
#include "timer.h"
#include "log.h"
#include <time.h>
#include <assert.h>
#include <signal.h>

#define INVALID_SOCKET -1

#define EVENT_TOTAL_COUNT	100000

#define MAX_DESCRIPTORS     100000

bool event_loop::m_bRun = true;	

event_loop::event_loop()
{
    m_count_fd = 0;
    m_fd_index = 0;
    fds = NULL;
}

event_loop::~event_loop()
{	
    sockapi::SocketClose(m_listen_fd);

    sockapi::SocketClose(m_epoll_fd);
    free(m_epev_arr);

    free(fds);  
}

void event_loop::SigHandle(int signum)
{
    if(signum == SIGTERM || signum == SIGUSR1 || signum == SIGKILL)
    {
        log_error("recv signal: %d  will kill down \n",signum);
        event_loop::m_bRun = false;  
    } 
}

bool event_loop::InitSocket(int listen_port)
{
	m_listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listen_fd == INVALID_SOCKET)
		return false;

    sockapi::SocketReUse(m_listen_fd);
	sockapi::SocketNoBlock(m_listen_fd);

    int ret = sockapi::ServerListen(m_listen_fd,listen_port);
    if(ret < 0)
        return false;

	if( !InitEvent())
        return false;

	log_debug("Server start running, listen port:%d\n", listen_port);
	return true;	
}

bool event_loop::Run()
{
	int loop_times = 0;
    const int timer_check_point = 10;

    while(m_bRun) 
    {
        int res = epoll_wait(m_epoll_fd, m_epev_arr, EVENT_TOTAL_COUNT, 100);
        if ( res < 0) {
            if (EINTR == errno)
                continue;
            log_debug("epoll_wait return false, errno = %d\n", errno);
            break;
        }
        else if (0 == res) { 
            loop_times = 0;
            run_timer();
        }
        else {
            if (++loop_times >= timer_check_point) {
                loop_times = 0;
                run_timer();
            }
        }
        
        for(int i=0; i<res; i++)
        {
            if(m_epev_arr[i].data.fd == m_listen_fd)
            {
                handle_accept();
                continue;
            }
            int fd = (uint32_t)m_epev_arr[i].data.u64; /* mask out the lower 32 bits */
            uint32_t index = (uint32_t)(m_epev_arr[i].data.u64 >> 32);
            tcpconn* s = fds[fd];
            if( s == 0 || s->getfdidx() != index )
            {                      
                continue;       // epoll returned invalid fd 
            }
            if( m_epev_arr[i].events & ( EPOLLHUP | EPOLLERR ))
            {    
                handle_close(s);
                continue;
            }
            else if(m_epev_arr[i].events & EPOLLIN )
            {               
                if( s->handle_read() == -1 )
                {
                    handle_close(s);
                    continue;
                }
                if( s->Writable() )
                    WantWrite(s);
            }
            else if( m_epev_arr[i].events & EPOLLOUT )
            {
                if( s->handle_write() == -1 )
                {
                    handle_close(s);
                    continue;
                }
                if( !s->Writable() )
                    WantRead(s);
            }
        }
    }
    return true;
}

bool event_loop::InitEvent()
{
    fds =  (tcpconn**)malloc(MAX_DESCRIPTORS * sizeof(void*));
    memset(fds,0,MAX_DESCRIPTORS * sizeof(void*));

	struct rlimit rl;
	int nfiles = MAX_DESCRIPTORS;
	if (getrlimit(RLIMIT_NOFILE, &rl) == 0 &&
		rl.rlim_cur != RLIM_INFINITY) {
			nfiles = rl.rlim_cur - 1;
	}
    if( nfiles > MAX_DESCRIPTORS )
        nfiles = MAX_DESCRIPTORS;

	log_debug("epoll create files:%d\n", nfiles);
	if (-1 == (m_epoll_fd = epoll_create(nfiles)))
		return false;
	
	struct epoll_event ev;
	ev.data.fd = m_listen_fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, m_listen_fd, &ev);

	m_epev_arr = (struct epoll_event*)malloc(EVENT_TOTAL_COUNT * sizeof(struct epoll_event));

    signal(SIGTERM, event_loop::SigHandle);    	
    signal(SIGUSR1,event_loop::SigHandle);
	signal(SIGKILL,event_loop::SigHandle);

	return true;
}

int event_loop::handle_accept()
{
	int conn_fd;
    do 
    {
        if((conn_fd = sockapi::ServerAccept(m_listen_fd)) == INVALID_SOCKET)
        {
            break;
        }
        sockapi::SetSocketMem(conn_fd,16*1024);
        if(sockapi::SocketNoBlock(conn_fd) < 0)
        {
            log_error("SetNonblock faild \n");
            sockapi::SocketClose(conn_fd);
            assert(false);
            continue;
        }
        if(sockapi::SetTcpKeepLive(conn_fd) < 0)
        {
            log_error("SetTcpKeepLive faild \n");
            sockapi::SocketClose(conn_fd);
            assert(false);
            continue;
        }	
        
        tcpconn* sh = AllocSocketHandler(conn_fd);
        if(sh == NULL)
        {
            log_error("sh is null \n");
            sockapi::SocketClose(conn_fd);
            assert(false);
            continue;
        }
        AddSocket(sh);
        sh->handle_connect();
    } while(conn_fd > 0);

	return 0;
}

void event_loop::handle_close(tcpconn* pHandler)
{
    assert(pHandler != NULL);
    pHandler->handle_close();

    RemoveSocket(pHandler);

    if(pHandler->getneeddel())
    {
        delete pHandler;
        pHandler = NULL;
    }
}

tcpconn* event_loop::AllocSocketHandler(int sock_fd)
{
	tcpconn* sh = CreateHandler();
	if(sh != NULL)
	{
        sh->setneeddel(true);
		sh->setfd(sock_fd);		
		sh->evloop(this);
	}
	return sh;
}
bool event_loop::DisConnect(tcpconn* pSocketHandler)
{
    log_debug("disconnect \n");
    handle_close(pSocketHandler);
 	return true;
}
bool event_loop::Register(tcpconn* pHandler)
{
    if(pHandler == NULL)
        return false;

    AddSocket(pHandler);

    pHandler->evloop(this);
    pHandler->handle_connect();	

	return true;
}

void event_loop::AddSocket(tcpconn* s)
{
    m_count_fd++;
    m_fd_index++;

    s->setfdidx(m_fd_index);

    assert( s->getfd() < MAX_DESCRIPTORS );
    assert(fds[s->getfd()] == 0);
    fds[s->getfd()] = s;
    
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));

    /* store the generation counter in the upper 32 bits, the fd in the lower 32 bits */
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLIN;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, s->getfd(), &ev);
}

void event_loop::RemoveSocket(tcpconn* s)
{
    m_count_fd--;

    assert(fds[s->getfd()] == s);
    fds[s->getfd()] = 0;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events =  EPOLLOUT | EPOLLIN;

    epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, s->getfd(), &ev);

    sockapi::SocketClose(s->getfd());
}

void event_loop::WantWrite(tcpconn* s)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLOUT ;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, s->getfd(), &ev);
}

void event_loop::WantRead(tcpconn* s)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLIN ;
    epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, s->getfd(), &ev);
}
