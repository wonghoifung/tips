#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include "ae.h"
#include "anet.h"

#define PORT 4444
#define MAX_LEN 1024

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
	printf("Test Output: %d\n", i++);
	// run after 10 seconds
	return 10000;
}

void close_handler(aeEventLoop *el, int fd, int err)
{
	if( 0 == err )
		printf("Client quit: %d\n", fd);
	else if( -1 == err )
		printf("quit abnormally\n");

	aeDeleteFileEvent(el, fd, AE_READABLE);
	close(fd);
}

void read_handler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	char buffer[MAX_LEN] = { 0 };
	int res;
	res = read(fd, buffer, MAX_LEN);
	if( res <= 0 )
	{
		close_handler(el, fd, res);
	}
	else
	{
		res = write(fd, buffer, MAX_LEN);
		if( -1 == res )
			close_handler(el, fd, res);
	}
}

void accept_handler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	int cfd, cport;
	char ip_addr[128] = { 0 };
	cfd = anetTcpAccept(errstr, fd, ip_addr, sizeof ip_addr, &cport);
	printf("Connected from %s:%d\n", ip_addr, cport);

	if( aeCreateFileEvent(el, cfd, AE_READABLE, read_handler, NULL) == AE_ERR )
	{
		fprintf(stderr, "client connect fail: %d\n", fd);
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

	int fd = anetTcpServer(errstr, PORT, NULL, 5);
	if( ANET_ERR == fd )
		fprintf(stderr, "Open port %d error: %s\n", PORT, errstr);

	if( aeCreateFileEvent(evloop, fd, AE_READABLE, accept_handler, NULL) == AE_ERR )
		fprintf(stderr, "Unrecoverable error creating server.ipfd file event.");

	aeCreateTimeEvent(evloop, 1, timeout_handler, NULL, NULL);

	aeMain(evloop);

	aeDeleteEventLoop(evloop);

	return 0;
}