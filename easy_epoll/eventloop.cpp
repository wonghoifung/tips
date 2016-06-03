#include "eventloop.h"
#include "log.h"
#include <sys/resource.h>
#include <stdlib.h>
#include <unistd.h>

eventloop::eventloop(): epollfd(-1), firedevents(NULL), stop(0)
{

}

eventloop::~eventloop()
{
	if (epollfd != -1) { close(epollfd); epollfd = -1; }
	if (firedevents) { free(firedevents); firedevents = NULL; }
}

bool eventloop::init(int maxfdcnt)
{
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

	return true;
}

void eventloop::run()
{
	while (!stop) {

	}
}

void eventloop::toread(int fd, uint32_t idx)
{
    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)fd | ((uint64_t)idx << 32);
    ev.events = EPOLLIN ;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}

void eventloop::towrite(int fd, uint32_t idx)
{
    struct epoll_event ev = { 0 };
    ev.data.u64 = (uint64_t)(uint32_t)fd | ((uint64_t)idx << 32);
    ev.events = EPOLLOUT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
}


