#include "MuteServer.h"
#include "sockapi.h"
#include "commands.h"
#include "message.h"
#include "tcpconn.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

static MuteServer* sc = NULL;
static timer gtimer;
static void startTimer() {
	gtimer.start(5);
}
static bool timerInit = false;
class tmp: public timer_handler {
public:
	int on_timeout(int tid) {
		outmessage msg;
		msg.begin(cmd_echo);
		msg.write_cstring("hello world");
		msg.end();
		if (sc) sc->send(&msg);
		startTimer();
		return 0;
	}
};
static bool initTimer() {
	if (timerInit) return true;
	static tmp handler_;
	timerInit = true;
	gtimer.set_handler(&handler_);
	return true;
}

MuteServer::MuteServer() {
	initTimer();
	sc = this;
	conn_ = new tcpconn(1);
	conn_->setneeddel(false);// TODO
}

MuteServer::~MuteServer() {
	if (conn_) {
		delete conn_;
		conn_ = NULL;
	}
}

bool MuteServer::init() {
	return true;
}

bool MuteServer::connect(const std::string& host, const std::string& port) {
	printf("%s\n", __FUNCTION__);
	connector_.set_evloop(this);
	bool ret = connector_.connect(conn_, host, atoi(port.c_str()));
	if (!ret) {
		printf("connect error\n");
	}
	return ret;
}

int MuteServer::send(outmessage* msg) {
	printf("%s\n", __FUNCTION__);
	return conn_->sendbuf(msg->cbuffer(), msg->size());
}

void MuteServer::handle_connect(tcpconn* conn) {
	printf("MuteServer::%s\n", __FUNCTION__);
	outmessage msg;
	msg.begin(cmd_peer_login);
	msg.write_int((int)getpid());
	msg.end();
	conn_->sendmsg(&msg);
}

void MuteServer::handle_disconnect(tcpconn* conn) {
	printf("MuteServer::%s\n", __FUNCTION__);
}

int MuteServer::handle_timeout(tcpconn*) {
	printf("MuteServer::%s\n", __FUNCTION__);
	return 0;
}

int MuteServer::handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid) {
	printf("MuteServer::%s\n", __FUNCTION__);
	const short cmd = msg->command();
	switch (cmd) {
		case cmd_peer_login: {
			int retcode = msg->read_int();
			printf("login ret: %d\n", retcode);
			if (retcode == -1) exit(0);
			startTimer();
			break;
		}
		case cmd_echo: {
			std::string rsp = msg->read_cstring();
			printf("recv: %s\n", rsp.c_str());
		}
	}
	return 0;
}
