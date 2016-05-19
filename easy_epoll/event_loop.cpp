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

bool event_loop::run_ = true;	

event_loop::event_loop():fdconns_(NULL),fdcount_(0),fdidx_(0),epevarr_(NULL)
{

}

event_loop::~event_loop()
{	
    socket_close(listen_sockfd_);
    socket_close(epollfd_);
    free(epevarr_);
    free(fdconns_);  
}

void event_loop::handle_signal(int signum)
{
    if(signum == SIGTERM || signum == SIGUSR1 || signum == SIGKILL)
    {
        log_error("recv signal: %d  will kill down \n",signum);
        event_loop::run_ = false;  
    } 
}

bool event_loop::init_server(int listen_port)
{
	listen_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sockfd_ == INVALID_SOCKET)
		return false;

    socket_reuse(listen_sockfd_);
	socket_nonblock(listen_sockfd_);

    int ret = socket_listen(listen_sockfd_,listen_port);
    if(ret < 0)
        return false;

	if( !init_event())
        return false;

	log_debug("Server start running, listen port:%d\n", listen_port);
	return true;	
}

bool event_loop::run()
{
	int loop_times = 0;
    const int timer_check_point = 10;

    while(run_) 
    {
        int res = epoll_wait(epollfd_, epevarr_, EVENT_TOTAL_COUNT, 100);
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
            if(epevarr_[i].data.fd == listen_sockfd_)
            {
                handle_accept();
                continue;
            }
            int fd = (uint32_t)epevarr_[i].data.u64; /* mask out the lower 32 bits */
            uint32_t index = (uint32_t)(epevarr_[i].data.u64 >> 32);
            tcpconn* s = fdconns_[fd];
            if( s == 0 || s->getfdidx() != index )
            {                      
                continue;       // epoll returned invalid fd 
            }
            if( epevarr_[i].events & ( EPOLLHUP | EPOLLERR ))
            {    
                handle_close(s);
                continue;
            }
            else if(epevarr_[i].events & EPOLLIN )
            {               
                if( s->handle_read() == -1 )
                {
                    handle_close(s);
                    continue;
                }
                if( s->writable() )
                    towrite(s);
            }
            else if( epevarr_[i].events & EPOLLOUT )
            {
                if( s->handle_write() == -1 )
                {
                    handle_close(s);
                    continue;
                }
                if( !s->writable() )
                    toread(s);
            }
        }
    }
    return true;
}

bool event_loop::init_event()
{
    fdconns_ =  (tcpconn**)malloc(MAX_DESCRIPTORS * sizeof(void*));
    memset(fdconns_,0,MAX_DESCRIPTORS * sizeof(void*));

	struct rlimit rl;
	int nfiles = MAX_DESCRIPTORS;
	if (getrlimit(RLIMIT_NOFILE, &rl) == 0 &&
		rl.rlim_cur != RLIM_INFINITY) {
			nfiles = rl.rlim_cur - 1;
	}
    if( nfiles > MAX_DESCRIPTORS )
        nfiles = MAX_DESCRIPTORS;

	log_debug("epoll create files:%d\n", nfiles);
	if (-1 == (epollfd_ = epoll_create(nfiles)))
		return false;
	
	struct epoll_event ev;
	ev.data.fd = listen_sockfd_;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd_, EPOLL_CTL_ADD, listen_sockfd_, &ev);

	epevarr_ = (struct epoll_event*)malloc(EVENT_TOTAL_COUNT * sizeof(struct epoll_event));

    signal(SIGTERM, event_loop::handle_signal);    	
    signal(SIGUSR1,event_loop::handle_signal);
	signal(SIGKILL,event_loop::handle_signal);

	return true;
}

int event_loop::handle_accept()
{
	int conn_fd;
    do 
    {
        if((conn_fd = socket_accept(listen_sockfd_)) == INVALID_SOCKET)
        {
            break;
        }
        socket_buffer(conn_fd,16*1024);
        if(socket_nonblock(conn_fd) < 0)
        {
            log_error("SetNonblock faild \n");
            socket_close(conn_fd);
            assert(false);
            continue;
        }
        if(socket_keepalive(conn_fd) < 0)
        {
            log_error("socket_keepalive faild \n");
            socket_close(conn_fd);
            assert(false);
            continue;
        }	
        
        tcpconn* c = prepare_tcpconn(conn_fd);
        if(c == NULL)
        {
            log_error("c is null \n");
            socket_close(conn_fd);
            assert(false);
            continue;
        }
        addsock(c);
        c->handle_connect();
    } while(conn_fd > 0);

	return 0;
}

void event_loop::handle_close(tcpconn* conn)
{
    assert(conn != NULL);
    conn->handle_close();

    remsock(conn);

    if(conn->getneeddel())
    {
        delete conn;
        conn = NULL;
    }
}

tcpconn* event_loop::prepare_tcpconn(int sock_fd)
{
	tcpconn* sh = create_tcpconn();
	if(sh != NULL)
	{
        sh->setneeddel(true);
		sh->setfd(sock_fd);		
		sh->evloop(this);
	}
	return sh;
}

bool event_loop::disconnect(tcpconn* conn)
{
    log_debug("disconnect \n");
    handle_close(conn);
 	return true;
}

bool event_loop::manage(tcpconn* conn)
{
    if(conn == NULL)
        return false;

    addsock(conn);

    conn->evloop(this);
    conn->handle_connect();	

	return true;
}

void event_loop::addsock(tcpconn* s)
{
    fdcount_++;
    fdidx_++;

    s->setfdidx(fdidx_);

    assert( s->getfd() < MAX_DESCRIPTORS );
    assert(fdconns_[s->getfd()] == 0);
    fdconns_[s->getfd()] = s;
    
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));

    /* store the generation counter in the upper 32 bits, the fd in the lower 32 bits */
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLIN;
    epoll_ctl(epollfd_, EPOLL_CTL_ADD, s->getfd(), &ev);
}

void event_loop::remsock(tcpconn* s)
{
    fdcount_--;

    assert(fdconns_[s->getfd()] == s);
    fdconns_[s->getfd()] = 0;

    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events =  EPOLLOUT | EPOLLIN;

    epoll_ctl(epollfd_, EPOLL_CTL_DEL, s->getfd(), &ev);

    socket_close(s->getfd());
}

void event_loop::towrite(tcpconn* s)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLOUT ;
    epoll_ctl(epollfd_, EPOLL_CTL_MOD, s->getfd(), &ev);
}

void event_loop::toread(tcpconn* s)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.u64 = (uint64_t)(uint32_t)(s->getfd()) | ((uint64_t)(uint32_t)(s->getfdidx()) << 32);

    ev.events = EPOLLIN ;
    epoll_ctl(epollfd_, EPOLL_CTL_MOD, s->getfd(), &ev);
}
