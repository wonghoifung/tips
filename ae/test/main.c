#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include "ae.h"
#include "anet.h"

char errstr[1024];

aeEventLoop *evloop = NULL;

static void signal_handler(int sig) {
	(void)sig;
    aeStop(evloop);
}

void set_signalhandler(void) {
	signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
}

int timeout_handler(struct aeEventLoop *eventLoop, long long id, void *clientData)
{
	static int i = 0;
	printf("timeout: %d\n", i++);
	// run after 10 seconds
	return 10000;
}

void close_handler(aeEventLoop *el, int fd, int err)
{
	if (0 == err)
		printf("client quit: %d\n", fd);
	else if (-1 == err)
		printf("quit abnormally\n");

	aeDeleteFileEvent(el, fd, AE_READABLE);
	close(fd);
}

#define MAX_LEN 1024
void read_handler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	char buffer[MAX_LEN] = { 0 };
	int res;
	res = read(fd, buffer, MAX_LEN);
	if (res <= 0)
	{
		close_handler(el, fd, res);
	}
	else
	{
		res = write(fd, buffer, MAX_LEN);
		if (-1 == res)
			close_handler(el, fd, res);
	}
}

void accept_handler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	int cfd, cport;
	char ip_addr[128] = { 0 };
	cfd = anetTcpAccept(errstr, fd, ip_addr, sizeof ip_addr, &cport);
	printf("connected from %s:%d\n", ip_addr, cport);

	if (aeCreateFileEvent(el, cfd, AE_READABLE, read_handler, NULL) == AE_ERR)
	{
		fprintf(stderr, "client connect failed: %d\n", fd);
		close(fd);
	}
}

static void set_rlimit()
{
	struct rlimit rlim, rlim_new;

	if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = 100000;
		if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0)
		{
			printf("set rlimit file failure\n");
			exit(0);
		}
	}

	if (getrlimit(RLIMIT_CORE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
		{
			printf("set rlimit core failure\n");
			exit(0);
		}
	}
}

int main()
{
	set_rlimit();

	set_signalhandler();

	evloop = aeCreateEventLoop(100000);

	int fd = anetTcpServer(errstr, 8888, NULL, 5);
	if (ANET_ERR == fd)
		fprintf(stderr, "listen port %d error: %s\n", 8888, errstr);

	if (aeCreateFileEvent(evloop, fd, AE_READABLE, accept_handler, NULL) == AE_ERR)
		fprintf(stderr, "cannot create file event for fd: %d\n", fd);

	aeCreateTimeEvent(evloop, 1, timeout_handler, NULL, NULL);

	aeMain(evloop);

	aeDeleteEventLoop(evloop);

	return 0;
}