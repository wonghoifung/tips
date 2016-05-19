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

struct NetAddr
{
	int     port;
	std::string  host;
    NetAddr(){
        host = "";
        port = 0;
    }
	NetAddr(const NetAddr &addr){
        host = addr.host;
        port = addr.port;}
	NetAddr & operator=(const NetAddr& addr){
		host = addr.host;
        port=addr.port;
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
	static int ServerListen(int fd , int port);
	static int ServerAccept(int fd);
	static int SocketSend(int fd, const char* buf, size_t len);
	static int SocketRecv(int fd , void* buf , size_t len );
	static void SocketClose(int fd);
	static int ClientConnect(int fd, const char* ip , int port );
	static int SocketNoBlock(int fd);
	static int SocketReUse(int fd);
    static int SetTcpKeepLive(int fd);
	static int SocketInit(void);
    static void SetSocketMem(int fd,int iSize);
	static int ConnNoblock(int fd,const char* ip, int port);
	static int WaitForConnect(int seconds);
};

#endif
