#include "SocketApi.h"
#include <netinet/tcp.h>
#include <netinet/ip.h>

std::vector<int> SocketApi::sessions;
int SocketApi::maxnums = 0;

SocketApi::SocketApi(void)
{
}

SocketApi::~SocketApi(void)
{
}

int SocketApi::ServerListen(int fd, int port)
{
	struct sockaddr_in addr;
	memset( &addr , 0,sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( 0 != bind( fd , (struct sockaddr *)&addr , sizeof(addr) ) )
	{
		log_error("Error: Bind Faile bind() %s\n",strerror(errno));		
		return -1;		
	}

	if( 0 != listen( fd, MAX_LISTEN_QUEUE ) )
	{
		log_error("Error: Listen Faile listen(): %s\n", strerror(errno));		
		return -1;		
	}   

	return 0;
}

int SocketApi::ServerAccept(int fd)
{
	struct sockaddr_in clientaddr;
	socklen_t clilen;
	clilen = sizeof(clientaddr);
	int connfd;

	connfd = accept( fd , (struct sockaddr *)&clientaddr , &clilen );
	if( connfd < 0 )
	{
		return -1;
	}
	else
	{
        log_debug("connection from %s ,port %d \n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		return connfd;
	}
}

int SocketApi::SocketSend(int fd, const char* buf, size_t len)
{
	int ret	= send( fd, buf, len, 0);
    return ret;
}

int SocketApi::SocketRecv(int fd , void* buf , size_t len )
{
	int ret = recv( fd, (char*)buf, len, 0 );
    return ret;
}

void SocketApi::SocketClose( int fd )
{
	close(fd);
}

int SocketApi::ClientConnect(int fd, const char* ip , int port )
{
	struct sockaddr_in remote;
	memset(&remote, 0,sizeof(remote));
	remote.sin_family = AF_INET;
	remote.sin_port = htons(port);
	remote.sin_addr.s_addr = inet_addr(ip);

	if(0 != connect(fd, (struct sockaddr*)&remote, sizeof(remote)))
	{
		log_error("Error: Connect Faile connect(): %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int SocketApi::SocketNoBlock(int fd)
{
	int opts = fcntl(fd, F_GETFL);
	if( opts < 0 )
		return -1;
	opts = opts | O_NONBLOCK;
	if( fcntl(fd, F_SETFL, opts) < 0)
		return -1;

	return 0;
}

int SocketApi::SocketReUse(int fd)
{
	int opt = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));	
}

int SocketApi::SetTcpKeepLive( int fd )
{
    int keepAlive = 1;
    int keepIdle = 60;
    int keepInterval = 1;
    int keepCount = 3;

    int ret1 = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
    int ret2 = setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
    int ret3 = setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
    int ret4 = setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount)); 

    if(ret1 == 0 && ret2 == 0 && ret3 == 0 && ret4 == 0)
        return 0;
    else
        return -1;
}

int SocketApi::SocketInit(void)
{
	int fd = socket(AF_INET , SOCK_STREAM , 0);

	if( 0 > fd)
	{
		log_error("Error  socket() %s\n",strerror(errno));		
	}
	return fd;
}

void SocketApi::SetSocketMem(int fd,int iSize)
{
    int opt = iSize;
    socklen_t optlen = sizeof(opt);
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &opt, optlen);
    setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &opt, optlen);
}

int SocketApi::ConnNoblock(int fd, const char* ip, int port)
{
	struct sockaddr_in remote;
	memset( &remote, 0,sizeof(remote));
	remote.sin_family 		 = AF_INET;
	remote.sin_port   		 = htons(port);
	remote.sin_addr.s_addr = inet_addr(ip);
	
	if(fd > 0)
	{
		if(0 == SocketApi::SocketNoBlock(fd))
		{
			if(0 > connect(fd, (struct sockaddr*)&remote, sizeof(remote)))
			{	
				if(errno != EINPROGRESS)
				{	
					return -1;
				}  
			}
			SocketApi::sessions.push_back(fd);
			SocketApi::maxnums++;
			return 0;
		}
		else
		{
			log_debug("Error NoBlock Faile Fd %d \n", fd);
		}
	}
	else
	{
		log_debug("Error Fd %d \n", fd);
	}
	return -1;
}

int SocketApi::WaitForConnect(int seconds)
{
	if(SocketApi::maxnums <= 0)
	{
		log_debug("Notice: Finish\n");
		SocketApi::maxnums = 0;
		return 0;
	}
	
	int maxfd = 0;
	int error = 0;

	socklen_t errlen = sizeof(error);
	
	fd_set readset;
	fd_set writeset;	
	
	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	
	std::vector<int>::iterator iter = SocketApi::sessions.begin();
	for(; iter != SocketApi::sessions.end(); ++iter)
	{
		FD_SET(*iter, &readset);
		FD_SET(*iter, &writeset);
		if(maxfd < *iter)
		{
			maxfd = *iter;
		}
	}
	
	struct timeval tval;
	tval.tv_sec = seconds;
	tval.tv_usec = 0;
	
	int n = 0;
	if(0 == (n = select(maxfd + 1, &readset, &writeset, NULL, seconds ? &tval : NULL)))
	{
	
		for(iter = SocketApi::sessions.begin(); iter != SocketApi::sessions.end(); ++iter)
		{
			SocketClose(*iter);
		}		
		log_debug("Notice: Select Time Out!!!\n");
		SocketApi::sessions.clear();
		SocketApi::maxnums = 0;
		return 0;
	}
	else if(n < 0)
	{
		log_debug("Error: Select Error!!!\n");
		return -1;
	}
	
	for(iter = SocketApi::sessions.begin(); iter != SocketApi::sessions.end();)
	{
		if(FD_ISSET(*iter, &readset) || FD_ISSET(*iter, &writeset))
		{
			if(getsockopt(*iter, SOL_SOCKET, SO_ERROR, (char*)&error, &errlen) < 0)
			{
				log_debug("Error: Can not Connect To Server Ad Fd %d\n", *iter);
				iter = SocketApi::sessions.erase(iter);
				--SocketApi::maxnums;
				SocketClose(*iter);
			}
			else
			{
				int rtfd = *iter;
				iter = SocketApi::sessions.erase(iter);
				--SocketApi::maxnums;
				if(error != 0)
				{
					log_debug("Error: Connect To Server Ad Fd %d Faile %s\n", *iter, strerror(error));
				}
				else
				{
					log_debug("Notice: Connect To Server Ad Fd %d Success \n", *iter);
					return rtfd;
				}				
			}
			continue;
		}
		
		++iter;
	}

	return 0;
}