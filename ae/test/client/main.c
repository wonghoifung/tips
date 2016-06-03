#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ae.h"
#include "anet.h"

#define PORT 4444
#define MAX_LEN 1024

char g_err_string[1024];

aeEventLoop *g_event_loop = NULL;

int PrintTimer(struct aeEventLoop *eventLoop, long long id, void *clientData)
{
	static int i = 0;
	printf("Test Output: %d\n", i++);
	// run after 10 seconds
	return 10000;
}

void StopServer()
{
	aeStop(g_event_loop);
}

void ClientClose(aeEventLoop *el, int fd, int err)
{
	if( 0 == err )
		printf("Client quit: %d\n", fd);
	else if( -1 == err )
		fprintf(stderr, "Client Error: %s\n", strerror(errno));

	aeDeleteFileEvent(el, fd, AE_READABLE);
	close(fd);
}

void ReadFromClient(aeEventLoop *el, int fd, void *privdata, int mask)
{
	printf("%s\n", __FUNCTION__);
	char buffer[MAX_LEN] = { 0 };
	int res;
	res = read(fd, buffer, MAX_LEN);
	if( res <= 0 )
	{
		ClientClose(el, fd, res);
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

	if(aeCreateFileEvent(el, fd, AE_READABLE, ReadFromClient, NULL) == AE_ERR)
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

	signal(SIGINT, StopServer);

	g_event_loop = aeCreateEventLoop(1024*10);

	int fd = anetTcpNonBlockConnect(g_err_string, "127.0.0.1", 4444);
	if (ANET_ERR == fd) {
		fprintf(stderr, "connect failure: %s\n", g_err_string);
		return -1;
	}
	if (aeCreateFileEvent(g_event_loop, fd, AE_WRITABLE, connect_handler, NULL) == AE_ERR) {
		fprintf(stderr, "cannot create connect event\n");
		return -1;
	}

	aeCreateTimeEvent(g_event_loop, 1, PrintTimer, NULL, NULL);

	aeMain(g_event_loop);

	aeDeleteEventLoop(g_event_loop);

	return 0;
}