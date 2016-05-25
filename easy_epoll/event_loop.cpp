#include "sockapi.h"
#include "event_loop.h"
#include "timer.h"
#include "log.h"
#include <time.h>
#include <assert.h>
#include <signal.h>

#define INVALID_SOCKET -1

#define EVENT_TOTAL_COUNT 100000

#define MAX_DESCRIPTORS 100000

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
    if (signum == SIGTERM || signum == SIGUSR1 || signum == SIGKILL) {
        log_error("recv signal: %d,  exit event loop", signum);
        event_loop::run_ = false;  
    } 
}

bool event_loop::init_server(int listen_port)
{
	listen_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sockfd_ == INVALID_SOCKET) return false;

    socket_reuse(listen_sockfd_);
	socket_nonblock(listen_sockfd_);

    int ret = socket_listen(listen_sockfd_, listen_port);
    if (ret < 0) return false;

	if (!init_event()) return false;

	log_debug("server start, listen port: %d", listen_port);
	return true;	
}

bool event_loop::init_client(const std::string& host, int port)
{
    int sock_fd = socket_create();
    if (sock_fd < 0) return false;

    int ret = socket_nonblock_connect(sock_fd, host.c_str(), port);
    if (ret == -1) return false;

    tcpconn* c = create_tcpconn(); // subclass provide implementation
    c->setfd(sock_fd);     
    c->evloop(this);
    addsock(c);

    if (ret == -2) { // connecting...
        c->setconnecting();
        towrite(c);
    } 
    else if (ret == 0) { // connected!
        c->handle_connect();
    }
    return true;
}

bool event_loop::run()
{
	int loop_times = 0;
    const int timer_check_point = 10;

    while (run_) {
        int res = epoll_wait(epollfd_, epevarr_, EVENT_TOTAL_COUNT, 100);

        if ( res < 0) {
            if (EINTR == errno) continue;
            log_error("epoll_wait return false, errno: %d(%s)", errno, strerror(errno));
            break;
        }
        else if (0 == res) { // timeout... no fd become ready
            loop_times = 0;
            run_timer();
        }
        else {
            if (++loop_times >= timer_check_point) {
                loop_times = 0;
                run_timer();
            }
        }
        
        for (int i = 0; i < res; i++) {
            if (epevarr_[i].data.fd == listen_sockfd_) {
                handle_accept();
                continue;
            }

            int fd = (uint32_t)epevarr_[i].data.u64; // mask out the lower 32 bits 
            uint32_t index = (uint32_t)(epevarr_[i].data.u64 >> 32);
            tcpconn* s = fdconns_[fd];
            
            if ( s == 0 || s->getfdidx() != index ) {         
                if (s) log_error("invalide fd: %d, epev_idx: %u, tcpconn_idx: %u", fd, index, s->getfdidx());
                else log_error("invalide fd: %d, epev_idx: %u, tcpconn_idx: null", fd, index);

                continue; // epoll return invalid fd 
            }

            if (epevarr_[i].events & (EPOLLHUP | EPOLLERR)) { 
                handle_close(s);
                continue;
            }
            else if (epevarr_[i].events & EPOLLIN) { 
                if (s->handle_read() == -1) {
                    handle_close(s);
                    continue;
                }
                if (s->writable())
                    towrite(s); // write first...
            }
            else if ( epevarr_[i].events & EPOLLOUT ) {
                if (s->isconnecting()) {
                    s->handle_connect(); 
                } 
                else {
                    if (s->handle_write() == -1) { // maybe overload, write loop buffer full...
                        handle_close(s);
                        continue;
                    }
                    if (!s->writable()) // write done, read...
                        toread(s);
                }
            }
        }
    }
    return true;
}

bool event_loop::init_event()
{
    fdconns_ = (tcpconn**)malloc(MAX_DESCRIPTORS * sizeof(void*));
    memset(fdconns_, 0, MAX_DESCRIPTORS * sizeof(void*));

	struct rlimit rl;
	int nfiles = MAX_DESCRIPTORS;
	if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY) {
			nfiles = rl.rlim_cur - 1;
	}
    if( nfiles > MAX_DESCRIPTORS )
        nfiles = MAX_DESCRIPTORS;

	log_debug("epoll create files: %d", nfiles);
	if (-1 == (epollfd_ = epoll_create(nfiles)))
		return false;
	
	struct epoll_event ev;
	ev.data.fd = listen_sockfd_;
	ev.events = EPOLLIN | EPOLLET; // et mode for listenfd
	epoll_ctl(epollfd_, EPOLL_CTL_ADD, listen_sockfd_, &ev);

	epevarr_ = (struct epoll_event*)malloc(EVENT_TOTAL_COUNT * sizeof(struct epoll_event));

    signal(SIGTERM, event_loop::handle_signal);    	
    signal(SIGUSR1, event_loop::handle_signal);
	signal(SIGKILL, event_loop::handle_signal);

	return true;
}

int event_loop::handle_accept()
{
	int conn_fd;

    do {
        if((conn_fd = socket_accept(listen_sockfd_)) == INVALID_SOCKET) break;

        socket_buffer(conn_fd, 16 * 1024);

        if(socket_nonblock(conn_fd) < 0) {
            log_error("cannot set non block for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }

        if(socket_keepalive(conn_fd) < 0) {
            log_error("cannot set keepalive for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }	
        
        tcpconn* c = prepare_tcpconn(conn_fd);
        if(c) {
            log_error("cannot prepare tcpconn for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }

        addsock(c);

        c->handle_connect();
    } while(conn_fd > 0); // loop because of the et listenfd

	return 0;
}

void event_loop::handle_close(tcpconn* conn)
{
    assert(conn != NULL);
    conn->handle_close();

    remsock(conn);

    if(conn->getneeddel()) {
        delete conn;
        conn = NULL;
    }
}

tcpconn* event_loop::prepare_tcpconn(int sock_fd)
{
	tcpconn* c = create_tcpconn(); // subclass provide implementation
	if (c) {
        c->setneeddel(true);
		c->setfd(sock_fd);		
		c->evloop(this);
	}
	return c;
}

bool event_loop::disconnect(tcpconn* conn)
{
    log_debug("initiate to disconnect, fd:%d, idx:%u", conn->getfd(), conn->getfdidx());
    handle_close(conn);
 	return true;
}

bool event_loop::manage(tcpconn* conn) // TODO no use
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

    // store the generation counter in the upper 32 bits, the fd in the lower 32 bits 
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

    ev.events = EPOLLOUT;
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
