#ifndef EVENTLOOP_HEADER
#define EVENTLOOP_HEADER

#include <sys/epoll.h>
#include <stdint.h>

struct eventloop
{
	int epollfd;
	struct epoll_event* firedevents;
	int stop;

	eventloop();
	~eventloop();
	bool init(int maxfdcnt);
	void run();
	void toread(int fd, uint32_t idx);
	void towrite(int fd, uint32_t idx);

private:
	eventloop(const eventloop&);
	eventloop& operator=(const eventloop&);
};

#endif
