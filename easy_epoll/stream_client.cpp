#include "stream_client.h"
#include "sockapi.h"
#include "message.h"
#include "tcpconn.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

stream_client::stream_client() {
	conn_ = new tcpconn(1);
	conn_->setneeddel(false);
}

stream_client::~stream_client() {
	if (conn_) {
		delete conn_;
		conn_ = NULL;
	}
}

tcpconn* stream_client::create_tcpconn(void) {
	return conn_;
}

bool stream_client::init() {
	return true;
}

bool stream_client::connect(const std::string& host, const std::string& port) {
	bool ret = connect(conn_, host, atoi(port.c_str()));
	if (!ret) {
		printf("connect error\n");
	}
	return ret;
}

int stream_client::send(outmessage* msg) {
	return conn_->sendbuf(msg->cbuffer(), msg->size());
}

void stream_client::handle_connect_event(tcpconn* conn) {
	socket_buffer(conn->getfd(),16*1024);
	socket_keepalive(conn->getfd()); // TODO

	on_connect(conn);
}

void stream_client::handle_disconnect_event(tcpconn* conn) {
	on_disconnect(conn);
}

int stream_client::handle_timeout_event(tcpconn* conn) {
	on_no_message(conn);
	return 0;
}

int stream_client::handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid) {
	on_message(msg,conn,ssid);
	return 0;
}

bool stream_client::set_evloop(event_loop* pServer) {
	return true;
}

bool stream_client::connect(tcpconn* conn, const std::string& strAddr, int port) {
	return init_client(strAddr, port);
}

bool stream_client::connect(tcpconn* conn, const address& addr) {
	return connect(conn,addr.host,addr.port);
}

bool stream_client::register_to_evloop(tcpconn* conn) { // no use TODO
	return manage(conn);
}
