#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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
		fprintf(stderr, "client error: %s\n", strerror(errno));

	aeDeleteFileEvent(el, fd, AE_READABLE);
	close(fd);
}

#define MAX_LEN 1024
void read_handler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	printf("%s\n", __FUNCTION__);
	char buffer[MAX_LEN] = { 0 };
	int res;
	res = read(fd, buffer, MAX_LEN);
	if (res <= 0)
	{
		close_handler(el, fd, res);
	}
	else
	{
		printf("received: %s\n", buffer);
	}
}

void connect_handler(aeEventLoop* el, int fd, void* privdata, int mask) {
	printf("%s\n", __FUNCTION__);
	aeDeleteFileEvent(el, fd, AE_WRITABLE);

	const char* buf = "helloworld";

	if (-1 == write(fd, buf, strlen(buf))) {
		fprintf(stderr, "cannot write\n");
		close(fd);
	}

	if (aeCreateFileEvent(el, fd, AE_READABLE, read_handler, NULL) == AE_ERR)
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

	evloop = aeCreateEventLoop(1024*10);

	int fd = anetTcpNonBlockConnect(errstr, "127.0.0.1", 8888);
	if (ANET_ERR == fd) {
		fprintf(stderr, "connect failure: %s\n", errstr);
		return -1;
	}
	if (aeCreateFileEvent(evloop, fd, AE_WRITABLE, connect_handler, NULL) == AE_ERR) {
		fprintf(stderr, "cannot create connect event\n");
		return -1;
	}

	aeCreateTimeEvent(evloop, 1, timeout_handler, NULL, NULL);

	aeMain(evloop);

	aeDeleteEventLoop(evloop);

	return 0;
}