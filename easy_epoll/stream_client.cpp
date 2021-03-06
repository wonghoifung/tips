#include "stream_client.h"
#include "sockapi.h"
#include "message.h"
#include "tcpconn.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

stream_client::stream_client(eventloop* evloop): evloop_(evloop) {
	conn_ = new tcpconn(1);
	conn_->setneeddel(false);
}

stream_client::~stream_client() {
	if (conn_) {
		// evloop_->delfd(conn_);
		delete conn_;
		conn_ = NULL;
	}
	evloop_ = NULL;
}

tcpconn* stream_client::create_tcpconn() {
	return conn_;
}

bool stream_client::connect(const std::string& host, const std::string& port) {
	bool ret = connect(host, atoi(port.c_str()));
	if (!ret) log_error("connect error");
	return ret;
}

bool stream_client::connect(const std::string& host, int port) {
	int sock_fd = socket_create();
    if (sock_fd < 0) return false;

    int ret = socket_nonblock_connect(sock_fd, host.c_str(), port);
    if (ret == -1) return false;

    tcpconn* c = create_tcpconn();
    c->setfd(sock_fd);     
    c->evloop(evloop_);
    c->evhandler(this);
    evloop_->addfd(c);

    if (ret == -2) { // connecting...
        c->setconnecting();
        evloop_->towrite(c);
    } 
    else if (ret == 0) { // connected!
        c->handle_connect();
    }
    return true;
}

int stream_client::send(outmessage* msg) {
	return conn_->sendbuf(msg->cbuffer(), msg->size());
}

void stream_client::handle_connect_event(tcpconn* conn) {
	socket_buffer(conn->getfd(), 16*1024);
	socket_keepalive(conn->getfd()); 

	on_connect(conn);
}

void stream_client::handle_disconnect_event(tcpconn* conn) {
	on_disconnect(conn);
}

int stream_client::handle_timeout_event(tcpconn* conn) {
	return on_no_message(conn);
}

int stream_client::handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid) {
	return on_message(msg,conn,ssid);
}
