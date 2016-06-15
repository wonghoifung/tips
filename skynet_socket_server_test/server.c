#include "socket_server.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

int fd[2];
fd_set rfds;

static void send_request(char type, char* buf, int blen) {
	int len = sizeof(char)+blen;
	char buffer[256];
	buffer[0] = type;
	memcpy(buffer+1,buf,blen);

	for (;;) {
		int n = write(fd[1], buffer, len);
		if (n<0) {
			if (errno != EINTR) {
				fprintf(stderr, "socket-server : send ctrl command error %s.\n", strerror(errno));
			}
			continue;
		}
		assert(n == len);
		return;
	}
}

static void *_poll(void * ud) {
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
				printf("%s\n", result.data);	
				#define DBUFLEN (sizeof(int)+sizeof(char*))
				char buf[DBUFLEN];
				memcpy(buf,(char*)&result.id,sizeof(int));
				memcpy(buf+sizeof(int),(char*)&(result.data),sizeof(char*));
				send_request('D',buf,DBUFLEN);
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
				send_request('A',(char*)&result.ud,sizeof(result.ud));
				break;
		}
	}
}

static void block_readpipe(int pipefd, void *buffer, int sz) {
	for (;;) {
		int n = read(pipefd, buffer, sz);
		if (n<0) {
			if (errno == EINTR)
				continue;
			fprintf(stderr, "read pipe error %s.\n",strerror(errno));
			return;
		}
		// must atomic read from a pipe
		assert(n == sz);
		return;
	}
}

static void test(struct socket_server *ss) {
	pipe(fd);

	pthread_t pid;
	pthread_create(&pid, NULL, _poll, ss);

	int l = socket_server_listen(ss, 200, "127.0.0.1", 8888, 32);
	printf("listening %d\n", l);
	socket_server_start(ss, 201, l);

	while (1) {
		struct timeval tv = {0,0};
		int retval;

		FD_ZERO(&rfds);
		FD_SET(fd[0], &rfds);
		retval = select(fd[0]+1, &rfds, NULL, NULL, &tv);
		if (retval==1) {
			char type;
			block_readpipe(fd[0], (char*)&type, sizeof(char));
			if (type=='A') {
				int id;
				block_readpipe(fd[0], (char*)&id, sizeof(int));
				printf("type:%c, id:%d\n",type,id);
				socket_server_start(ss,700,id);
			} else if (type=='D') {
				char data[256];
				int id;
				char* str;
				block_readpipe(fd[0], &data, sizeof(int)+sizeof(char*));
				memcpy(&id, data, sizeof(int));
				str = *(char**)(data+sizeof(int));
				socket_server_send(ss,id,str,strlen(str));
				//free(str); // socket_server_send already free str
			}
		}
	}

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
