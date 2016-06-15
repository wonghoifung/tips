#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

static void * _poll(void * ud) {
	struct socket_server *ss = ud;
	struct socket_message result;
	for (;;) {
		int type = socket_server_poll(ss, &result, NULL);
		// DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
		switch (type) {
		case SOCKET_EXIT:
			return NULL;
		case SOCKET_DATA:
			printf("message(%lu) [id=%d] size=%d\n", result.opaque, result.id, result.ud);
			printf("%s\n",result.data);
			free(result.data);
			break;
		case SOCKET_CLOSE:
			printf("close(%lu) [id=%d]\n", result.opaque, result.id);
			break;
		case SOCKET_OPEN:
			printf("open(%lu) [id=%d] %s\n", result.opaque, result.id, result.data);
			break;
		case SOCKET_ERROR:
			printf("error(%lu) [id=%d]\n", result.opaque, result.id);
			break;
		case SOCKET_ACCEPT:
			printf("accept(%lu) [id=%d %s] from [%d]\n", result.opaque, result.ud, result.data, result.id);
			break;
		default:
			printf("default, type: %d\n", type);
		}
	}
}

#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0])) 

static void test(struct socket_server *ss) {
	pthread_t pid;
	pthread_create(&pid, NULL, _poll, ss);
	int i = 0;

	int connid = socket_server_connect(ss, 400, "127.0.0.1", 8888);
	
	getc(stdin);
	socket_server_start(ss, 401, connid);

	char* tmp[3] = { "hello", "world", "wong" };
	int cnt = ARRLEN(tmp);
	
	char* contents[ARRLEN(tmp)];
	for (i = 0; i < cnt; ++i)
	{
		contents[i] = malloc(strlen(tmp[i]) + 1); 
		memcpy(contents[i], tmp[i], strlen(tmp[i]));
	}
	
	for (i = 0; i < cnt; ++i)
	{
		socket_server_send(ss, connid, contents[i], strlen(contents[i]));
		getc(stdin);
	}
	printf("done\n");

	pthread_join(pid, NULL);
}

int main() {
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	struct socket_server * ss = socket_server_create();
	test(ss);
	socket_server_release(ss);

	return 0;
}
