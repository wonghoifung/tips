#include "SimpleClient.h"
#include "commands.h"
#include "timer.h"

static stream_client* sc = NULL;
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

SimpleClient::SimpleClient() {
	initTimer();
	sc = this;
}

SimpleClient::~SimpleClient() {

}

void SimpleClient::handle_connect(tcpconn* conn) {
	printf("SimpleClient::%s\n", __FUNCTION__);
	stream_client::handle_connect(conn);

	printf("stream_client::%s\n", __FUNCTION__);
	outmessage msg;
	msg.begin(cmd_peer_login);
	msg.write_int((int)getpid());
	msg.end();
	conn_->sendmsg(&msg);
}

void SimpleClient::handle_disconnect(tcpconn* conn) {
	printf("SimpleClient::%s\n", __FUNCTION__);
}

int SimpleClient::handle_timeout(tcpconn*) {
	printf("SimpleClient::%s\n", __FUNCTION__);
	return 0;
}

int SimpleClient::handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid) {
	printf("SimpleClient::%s\n", __FUNCTION__);
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
