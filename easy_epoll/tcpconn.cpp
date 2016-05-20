#include "tcpconn.h"
#include "log.h"
#include "sockapi.h"
#include "event_loop.h"
#include "message.h"
#include "stream_server.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>

tcpconn::tcpconn(int cid)
:timer_handler()
,sockfd_(0)
,fdidx_(0)
,full_(false)
,status_(-1)
,connid_(cid)
,remoteaddr_()
,port_(0)
,ud_(NULL)
,parser_(NULL)
{
    needdel_ = false;
	memset(recvbuf_,0,sizeof(recvbuf_));
	evloop_ = NULL;
    tcptimer_.set_handler(this);

    sendloopbuf_ = new loopbuf(MAX_LOOP_BUFFER_LEN);
}

tcpconn::~tcpconn()
{
	if (sendloopbuf_) {
		delete sendloopbuf_;
	   sendloopbuf_ = NULL;
    }
    if (parser_) {
        delete parser_;
        parser_ = NULL;
    }
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
        int nRecv = socket_recv(sockfd_,recvbuf_,buff_size);
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
        if(on_rawdata(recvbuf_, nRecv) != 0)
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
        nPeekLen = sendloopbuf_->peek(tmpsendbuf_,sizeof(tmpsendbuf_));
        nHaveSendLen = socket_send(getfd(),tmpsendbuf_, nPeekLen);

        //sendbuf data block
        if( nHaveSendLen < 0 ) 
        {
            if(errno != EWOULDBLOCK && errno != EINTR)
            {
                sendloopbuf_->erase(nPeekLen);            
                return -1;
            }
            return 0;
        }
        else
        {
            sendloopbuf_->erase(nHaveSendLen);
        }
     } while (nHaveSendLen > 0 && sendloopbuf_->datacount()>0);

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
    if( nLen > (int)sendloopbuf_->freecount())
    {
        log_debug("sendloopbuf_ not enough\n");
        full_ = true;
        return -1;
    }
    else        
	    sendloopbuf_->put((char *)buf, nLen);
    handle_write();   

    if(writable())
        evloop_->towrite(this);

	return 0;
}

bool tcpconn::writable()
{
    return sendloopbuf_->datacount() > 0;
}
//-----------------------
int tcpconn::sendmsg(outmessage* msg) {
    return sendbuf(msg->cbuffer(), msg->size());
}
void tcpconn::setremoteaddr(void) {
    sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    int len = sizeof(remote_addr);
    if(getpeername(getfd(), reinterpret_cast<sockaddr *> (&remote_addr), (socklen_t*)&len) == 0)
    {
        remoteaddr_ = inet_ntoa(remote_addr.sin_addr);
        port_ = ntohs(remote_addr.sin_port);
    }
}
int tcpconn::on_message(inmessage* msg) {
    stream_server *pServer = (stream_server *)this->evloop();
    return pServer->handle_message(msg, this, connid_);
}
int tcpconn::on_rawdata(char* buf, int nLen) {
    status_ = REQUEST;
    tcptimer_.stop();   

    if(parser_ == NULL)
        parser_ = message_parser::create(this);

    return parser_->parse(buf, nLen);
}
int tcpconn::on_close(void) {
    status_ = CLOSE;    
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->handle_disconnect(this);
    return 0;
}
int tcpconn::on_connect(void) {
    status_ = CONNECT;
    stream_server *pServer = (stream_server*)this->evloop();
    if(pServer != NULL)
        pServer->handle_connect(this);

    tcptimer_.start(30);
    setremoteaddr();
    return 0;
}
int tcpconn::on_timeout(int timerid) {
    stream_server *pServer = (stream_server*)this->evloop();
    int nRet = pServer->handle_timeout(this);
    return nRet;
}
