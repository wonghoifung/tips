#include <stdio.h>
#include <errno.h>
#include <signal.h>
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
	char buffer[MAX_LEN] = { 0 };
	int res;
	res = read(fd, buffer, MAX_LEN);
	if( res <= 0 )
	{
		ClientClose(el, fd, res);
	}
	else
	{
		res = write(fd, buffer, MAX_LEN);
		if( -1 == res )
			ClientClose(el, fd, res);
	}
}

void AcceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask)
{
	int cfd, cport;
	char ip_addr[128] = { 0 };
	cfd = anetTcpAccept(g_err_string, fd, ip_addr, sizeof ip_addr, &cport);
	printf("Connected from %s:%d\n", ip_addr, cport);

	if( aeCreateFileEvent(el, cfd, AE_READABLE, ReadFromClient, NULL) == AE_ERR )
	{
		fprintf(stderr, "client connect fail: %d\n", fd);
		close(fd);
	}
}

int main()
{
	printf("Start\n");

	signal(SIGINT, StopServer);

	g_event_loop = aeCreateEventLoop(1024*10);

	int fd = anetTcpServer(g_err_string, PORT, NULL, 5);
	if( ANET_ERR == fd )
		fprintf(stderr, "Open port %d error: %s\n", PORT, g_err_string);

	if( aeCreateFileEvent(g_event_loop, fd, AE_READABLE, AcceptTcpHandler, NULL) == AE_ERR )
		fprintf(stderr, "Unrecoverable error creating server.ipfd file event.");

	aeCreateTimeEvent(g_event_loop, 1, PrintTimer, NULL, NULL);

	aeMain(g_event_loop);

	aeDeleteEventLoop(g_event_loop);

	printf("End\n");
	return 0;
}