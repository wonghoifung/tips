#include "tcpconn.h"
#include "log.h"
#include "event_loop.h"
#include "message.h"
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
    ,needdel_(false)
    ,full_(false)
    ,evloop_(NULL)
    ,sendloopbuf_(new loopbuf(MAX_LOOP_BUFFER_LEN))
    ,status_(-1)
    ,connid_(cid)
    ,ud_(NULL)
    ,parser_(NULL)
{
	memset(recvbuf_, 0, sizeof(recvbuf_));
    memset(tmpsendbuf_, 0, sizeof(tmpsendbuf_));
    tcptimer_.set_handler(this);
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

void tcpconn::setremoteaddr() {
    sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    int len = sizeof(remote_addr);
    if (getpeername(getfd(), reinterpret_cast<sockaddr*>(&remote_addr), (socklen_t*)&len) == 0) {
        remoteaddr_ = inet_ntoa(remote_addr.sin_addr);
        int port = ntohs(remote_addr.sin_port);
        char bufport[32] = {0};
        sprintf(bufport,":%d",port);
        remoteaddr_ += bufport;
    }
}

int tcpconn::handle_connect()
{
    status_ = CONNECT;
    setremoteaddr();
    event_handler* h = (event_handler*)this->evloop();
    if (h) h->handle_connect_event(this);

    tcptimer_.start(30);
    return 0;
}

int tcpconn::handle_read()
{
    if (full_) {
        log_error("tcpconn::handle_read, overload, fd:%d, idx:%d, connid:%d", sockfd_, fdidx_, connid_);
        return -1;
    }
    const int buff_size = sizeof(recvbuf_);
    while (1) {
        int nRecv = socket_recv(sockfd_, recvbuf_, buff_size);

        if (nRecv < 0) {
            if (EAGAIN == errno || EWOULDBLOCK == errno) { return 0; }
            return -1;
        }

        if (nRecv == 0) return -1;

        if (process_rawdata(recvbuf_, nRecv) != 0) return -1;
        
        if (nRecv < buff_size) return 0;
    } 
    return -1;
}

int tcpconn::handle_write()
{
    if (!writable()) return 0;

    if (full_) {
        log_error("tcpconn::handle_write, overload, fd:%d, idx:%d, connid:%d", sockfd_, fdidx_, connid_);
        return -1;
    }

    int nPeekLen = 0;
    int nHaveSendLen = 0;
    do {
        nPeekLen = sendloopbuf_->peek(tmpsendbuf_, sizeof(tmpsendbuf_));
        nHaveSendLen = socket_send(getfd(), tmpsendbuf_, nPeekLen);

        if( nHaveSendLen < 0 ) {
            if(errno != EWOULDBLOCK && errno != EINTR) {
                sendloopbuf_->erase(nPeekLen);            
                return -1;
            }
            return 0;
        }
        else {
            sendloopbuf_->erase(nHaveSendLen);
        }
    } while (nHaveSendLen > 0 && sendloopbuf_->datacount() > 0);

    return 0;
}

int tcpconn::handle_close()
{
	tcptimer_.stop();

    status_ = CLOSE;    
    event_handler* h = (event_handler*)this->evloop();
    if (h) h->handle_disconnect_event(this);
    return 0;
}

int tcpconn::sendbuf(const char* buf, int nLen)
{
    if (nLen > (int)sendloopbuf_->freecount()) {
        log_error("tcpconn::sendbuf, sendloopbuf not enough, fd:%d, idx:%d, connid:%d", sockfd_, fdidx_, connid_);
        full_ = true; // no way to restore now
        return -1;
    }
    else {    
	    sendloopbuf_->put((char *)buf, nLen);
    }

    handle_write(); // do the write at once

    if (writable()) evloop_->towrite(this); // continue until sndloopbuf empty

	return 0;
}

int tcpconn::sendmsg(outmessage* msg) 
{
    return sendbuf(msg->cbuffer(), msg->size());
}

bool tcpconn::writable()
{
    return sendloopbuf_->datacount() > 0;
}

int tcpconn::process_message(inmessage* msg) {
    event_handler* h = (event_handler*)this->evloop();
    return h->handle_message_event(msg, this, connid_);
}

int tcpconn::process_rawdata(char* buf, int nLen) {
    status_ = REQUEST;
    tcptimer_.stop();   
    if (parser_ == NULL) parser_ = message_parser::create(this);
    return parser_->parse(buf, nLen);
}

int tcpconn::on_timeout(int timerid) {
    event_handler* h = (event_handler*)this->evloop();
    return h->handle_timeout_event(this);
}
