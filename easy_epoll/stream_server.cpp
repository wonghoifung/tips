#include "stream_server.h"
#include "sockapi.h"
#include <stdarg.h>
#include <assert.h>

#define INVALID_SOCKET -1

stream_server::stream_server(eventloop* evloop): event_handler(), evloop_(evloop), listenfd_(-1), maxid_(0)
{

}

stream_server::~stream_server()
{
    if (listenfd_ != -1) { 
        assert(evloop_);
        evloop_->dellistenfd(this);
        socket_close(listenfd_); 
        listenfd_ = -1; 
    }
}

bool stream_server::init(int listenport)
{
    assert(listenfd_ == -1);

    listenfd_ = socket_create();
    if (listenfd_ == INVALID_SOCKET) return false;

    socket_reuse(listenfd_);
    socket_nonblock(listenfd_);

    int ret = socket_listen(listenfd_, listenport);
    if (ret < 0) return false;

    evloop_->addlistenfd(this);

    // TODO handle signal

    log_debug("server start, listen port: %d", listenport);
    return true;
}

int stream_server::handle_accept()
{
    int conn_fd;

    do {
        if ((conn_fd = socket_accept(listenfd_)) == INVALID_SOCKET) break;

        socket_buffer(conn_fd, 16 * 1024);

        if (socket_nonblock(conn_fd) < 0) {
            log_error("cannot set non block for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }

        if (socket_keepalive(conn_fd) < 0) {
            log_error("cannot set keepalive for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }   
        
        tcpconn* c = create_tcpconn();
        if (c == NULL) {
            log_error("cannot allocate tcpconn for fd: %d", conn_fd);
            socket_close(conn_fd);
            assert(false);
            continue;
        }
        c->setneeddel(true);
        c->setfd(conn_fd);
        c->evloop(evloop_);
        c->evhandler(this);

        evloop_->addfd(c);

        c->handle_connect();
    } while(conn_fd > 0); // loop because of the et listenfd

    return 0;
}

tcpconn* stream_server::create_tcpconn()
{
	tcpconn* conn = NULL;
    int uid = genconnid();
	conn = new tcpconn(uid); // server: delete by event_loop
	return conn;
}

void stream_server::handle_connect_event(tcpconn* conn)
{
    int id = conn->connid();
    if(connmap_.find(id) == connmap_.end()) {
        connmap_.insert(std::map<int, tcpconn*>::value_type(id,conn));
    }
    else {
        log_debug("stream_server::handle_connect_event connid %d already existed", conn->connid());
        assert(false);
    }
    on_connect(conn); // subclass provide implementation
}

void stream_server::handle_disconnect_event(tcpconn* conn)
{
    int id = conn->connid();
    std::map<int, tcpconn*>::iterator iter = connmap_.find(id);
    if(iter != connmap_.end()) {
        connmap_.erase(iter);
    }
    else {
        log_debug("stream_server::handle_disconnect_event connid %d not existed", conn->connid());
        assert(false);
    }
    on_disconnect(conn); // subclass provide implementation
}

int stream_server::handle_timeout_event(tcpconn* conn)
{
    log_debug("connect 30s and no message, disconnect connid: %d", conn->connid());
    evloop_->close_conn(conn);
    return on_no_message(conn); // subclass provide implementation
}

int stream_server::handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid) 
{
    return on_message(msg, conn, ssid); // subclass provide implementation
}

tcpconn* stream_server::findconn(int idx)
{
	std::map<int, tcpconn*>::iterator iter = connmap_.find(idx);
	if(iter != connmap_.end()) {
		return iter->second;
	}
	return NULL;
}

int stream_server::genconnid()
{
    ++maxid_;
    while (connmap_.find(maxid_) != connmap_.end()) {
        ++maxid_;
    }
	return maxid_;
}

