#ifndef EVENTLOOP_HEADER
#define EVENTLOOP_HEADER

#include <sys/epoll.h>

struct eventloop
{
	int epollfd;
	struct epoll_event* firedevents;
	int stop;

	eventloop();
	~eventloop();
	bool init(int maxfdcnt);
	void run();
	
private:
	eventloop(const eventloop&);
	eventloop& operator=(const eventloop&);
};

#endif
