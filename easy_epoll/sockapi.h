#ifndef SOCKAPI_HEADER
#define SOCKAPI_HEADER

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>
#include <vector>
#include <iostream>
#include <string>
#include "log.h"

#define MAX_LISTEN_QUEUE 100000

struct address
{
	int port;
	std::string host;
    address() {
        host = "";
        port = 0;
    }
	address(const address& addr) {
        host = addr.host;
        port = addr.port;}
	address& operator=(const address& addr) {
		host = addr.host;
        port = addr.port;
        return *this;
	}
};

enum STATUS
{
    CONNECT=0, 
    REQUEST, 
    CLOSE
};

class sockapi
{
public:
	static std::vector<int> sessions;
	static int maxnums;

	sockapi(void);
	~sockapi(void);
	static int server_listen(int fd , int port);
	static int server_accept(int fd);
	static int socket_send(int fd, const char* buf, size_t len);
	static int socket_recv(int fd , void* buf , size_t len );
	static void socket_close(int fd);
	static int client_connect(int fd, const char* ip , int port );
	static int socket_nonblock(int fd);
	static int socket_reuse(int fd);
    static int socket_keepalive(int fd);
	static int socket_create(void);
    static void socket_buffer(int fd,int iSize);
	static int socket_nonblock_connect(int fd,const char* ip, int port);
	static int socket_wait_connect(int seconds);
};

#endif
