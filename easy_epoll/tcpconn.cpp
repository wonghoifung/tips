#include "tcpconn.h"
#include "log.h"
#include "sockapi.h"
#include "event_loop.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>

tcpconn::tcpconn()
:timer_handler()
,sockfd_(0)
,fdidx_(0)
,full_(false)
{
    needdel_ = false;
	memset(recvbuf_,0,sizeof(recvbuf_));
	evloop_ = NULL;
    tcptimer_.set_handler(this);

    sendloopbuf_ = new loopbuf(MAX_LOOP_BUFFER_LEN);
}

tcpconn::~tcpconn()
{
	if(sendloopbuf_)
		delete sendloopbuf_;
	sendloopbuf_ = NULL;
}

int tcpconn::handle_connect()
{
	return on_connect();
}

int tcpconn::handle_read()
{
    if(full_)
        return -1;

    const int buff_size = sizeof(recvbuf_);
    while(1) 
    {
        int nRecv = recv(sockfd_,recvbuf_,buff_size,0);
        if(nRecv < 0)
        {
            if(EAGAIN == errno || EWOULDBLOCK == errno)
            {           			
                return 0;
            }
            return -1;
        }
        if(nRecv == 0)
        {
            return -1;
        }
        int ret = on_rawdata(recvbuf_, nRecv);
        if(ret != 0)
            return -1;
        if(nRecv < buff_size)
            return 0;
    } 
    return -1;
}

int tcpconn::handle_write()
{
    if(!writable())
        return 0;
    if(full_)
        return -1;

    int nPeekLen = 0;
    int nHaveSendLen = 0;
    do 
    {
        nPeekLen = sendloopbuf_->Peek(tmpsendbuf_,sizeof(tmpsendbuf_));
        nHaveSendLen = sockapi::SocketSend(getfd(),tmpsendbuf_, nPeekLen);

        //sendbuf data block
        if( nHaveSendLen < 0 ) 
        {
            if(errno != EWOULDBLOCK && errno != EINTR)
            {
                sendloopbuf_->Erase(nPeekLen);            
                return -1;
            }
            return 0;
        }
        else
        {
            sendloopbuf_->Erase(nHaveSendLen);
        }
     }while (nHaveSendLen>0 && sendloopbuf_->DataCount()>0);

   return 0;
}

int tcpconn::handle_close()
{
	tcptimer_.stop();
	on_close();
	return 0;
}

int tcpconn::sendbuf(const char *buf, int nLen)
{
    if( nLen > (int)sendloopbuf_->FreeCount())
    {
        log_debug("SendLoopBuff not enough\n");
        full_ = true;
        return -1;
    }
    else        
	    sendloopbuf_->Put((char *)buf, nLen);
    handle_write();   

    if(writable())
        evloop_->WantWrite(this);

	return 0;
}

bool tcpconn::writable()
{
    return sendloopbuf_->DataCount() > 0;
}


