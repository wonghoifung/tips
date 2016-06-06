#include "eventloop.h"
#include "log.h"
#include "sockapi.h"
#include "timer.h"
#include "tcpconn.h"
#include "stream_server.h"
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

eventloop::eventloop()
	: stop(0), 
	epollfd(-1), 
	firedevents(NULL), 
	maxfdcount(100000), 
	fdcount(0), 
	fdidx(0),
	fdconns(NULL),
	listenfd(-1)
{

}

eventloop::~eventloop()
{
	if (epollfd != -1) { close(epollfd); epollfd = -1; }
	if (firedevents) { free(firedevents); firedevents = NULL; }
	if (fdconns) { free(fdconns); fdconns = NULL; }
}

bool eventloop::init(int maxfdcnt)
{
	maxfdcount = maxfdcount;

	struct rlimit rl;
	int filenum = maxfdcnt;
	if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY) {
		filenum = rl.rlim_cur - 1;
	}
	if (filenum > maxfdcnt) filenum = maxfdcnt;
	log_debug("epoll create hint filenum: %d", filenum);

	epollfd = epoll_create(filenum); // it's just a hint
	if (-1 == epollfd) {
		log_error("cannot create epoll fd");
		return false;
	}
	
	firedevents = (struct epoll_event*)malloc(maxfdcnt * sizeof(struct epoll_event));
	if (NULL == firedevents) {
		log_error("cannot allocate space for firedevents");
		return false;
	}

	fdconns = (tcpconn**)malloc(maxfdcount * sizeof(void*));
	if (NULL == fdconns) {
		log_error("cannot allocate space for fdconns");
		return false;
	}
    memset(fdconns, 0, maxfdcount * sizeof(void*));

	return true;
}

void eventloop::run()
{
	int loop_times = 0;
    const int timer_check_point = 10;
	while (!stop) {
		int res = epoll_wait(epollfd, firedevents, maxfdcount, 100);
		if ( res < 0) {
			if (EINTR == errno) continue;
			log_error("epoll_wait report error, errno: %d(%s)", errno, strerror(errno));
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
            if (firedevents[i].data.fd == listenfd) {
            	stream_server* server = (stream_server*)(void*)fdconns[listenfd];
            	assert(server);
                server->handle_accept();
                continue;
            }

            int fd = (uint32_t)firedevents[i].data.u64; // mask out the lower 32 bits 
            uint32_t index = (uint32_t)(firedevents[i].data.u64 >> 32);
            tcpconn* tc = fdconns[fd];
            
            if ( tc == 0 || tc->getfdidx() != index ) {         
                if (tc) log_error("invalide fd: %d, epev_idx: %u, tcpconn_idx: %u", fd, index, tc->getfdidx());
                else log_error("invalide fd: %d, epev_idx: %u, tcpconn_idx: null", fd, index);

                continue; // epoll return invalid fd 
            }

            if (firedevents[i].events & (EPOLLHUP | EPOLLERR)) { 
                close_conn(tc);
                continue;
            }
            else if (firedevents[i].events & EPOLLIN) { 
                if (tc->handle_read() == -1) { // maybe overload, write loop buffer full...
                    close_conn(tc);
                    continue;
                }
                if (tc->writable())
                    towrite(tc); // write first...
            }
            else if (firedevents[i].events & EPOLLOUT) {
                if (tc->isconnecting()) {
                    tc->handle_connect(); 
                } 
                else {
                    if (tc->handle_write() == -1) { // maybe overload, write loop buffer full...
                        close_conn(tc);
                        continue;
                    }
                    if (!tc->writable()) // write done, read...
                        toread(tc);
                }
            }
        }
	}
}

void eventloop::toread(tcpconn* tc)
{
    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)(tc->getfd()) | ((uint64_t)(uint32_t)(tc->getfdidx()) << 32);
    ev.events = EPOLLIN ;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, tc->getfd(), &ev);
}

void eventloop::towrite(tcpconn* tc)
{
    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)(tc->getfd()) | ((uint64_t)(uint32_t)(tc->getfdidx()) << 32);
    ev.events = EPOLLOUT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, tc->getfd(), &ev);
}

void eventloop::addlistenfd(stream_server* server)
{
	assert(listenfd == -1);

	assert(server->listenfd() < maxfdcount);
	assert(fdconns[server->listenfd()] == 0);
	fdconns[server->listenfd()] = (tcpconn*)(void*)server; // hack

	struct epoll_event ev = { 0 };
	ev.data.fd = server->listenfd();
	ev.events = EPOLLIN | EPOLLET; // et mode for listenfd
	epoll_ctl(epollfd, EPOLL_CTL_ADD, server->listenfd(), &ev);

	listenfd = server->listenfd();
}

void eventloop::dellistenfd(stream_server* server)
{
	assert(listenfd != -1);

	assert(fdconns[server->listenfd()] == (tcpconn*)(void*)server);
	fdconns[server->listenfd()] = 0;

	struct epoll_event ev = { 0 };
    ev.data.fd = server->listenfd();
    ev.events = EPOLLOUT | EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, server->listenfd(), &ev);
}

void eventloop::addfd(tcpconn* tc)
{
    fdcount += 1;
    fdidx += 1;

    tc->setfdidx(fdidx);

    assert(tc->getfd() < maxfdcount);
    assert(fdconns[tc->getfd()] == 0);
    fdconns[tc->getfd()] = tc;

    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)(tc->getfd()) | ((uint64_t)(uint32_t)(tc->getfdidx()) << 32);
    ev.events = EPOLLIN ;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, tc->getfd(), &ev);
}

void eventloop::delfd(tcpconn* tc)
{
    fdcount -= 1;

    assert(fdconns[tc->getfd()] == tc);
    fdconns[tc->getfd()] = 0;

    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)(tc->getfd()) | ((uint64_t)(uint32_t)(tc->getfdidx()) << 32);
    ev.events = EPOLLOUT | EPOLLIN;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, tc->getfd(), &ev);
    socket_close(tc->getfd());
}

void eventloop::close_conn(tcpconn* conn)
{
	assert(conn != NULL);
	conn->handle_close();

	delfd(conn);

	if (conn->getneeddel()) {
		delete conn;
		conn = NULL;
	}
}
