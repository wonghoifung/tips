#include "SimpleClient.h"
#include "commands.h"
#include "timer.h"

static stream_client* sc = NULL;
static timer gtimer;
static void startTimer() {
	gtimer.start(5);
}
static bool timerInit = false;
static int cc = 0;
class tmp: public timer_handler {
public:
	int on_timeout(int tid) {
		outmessage msg;
		msg.begin(cmd_echo);
		msg.write_cstring("hello world");
		msg.end();
		if (sc) sc->send(&msg);

		{
			outmessage msg;
			msg.begin(cmd_upper);
			msg.write_cstring("hello world");
			msg.end();
			if (sc) sc->send(&msg);
		}

		if (cc++ % 2 == 0) {
			outmessage msg;
			msg.begin(cmd_chat2all);
			msg.write_cstring("zbbbbbbbbbbz");
			msg.end();
			if (sc) sc->send(&msg);
		}

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

SimpleClient::SimpleClient(eventloop* evloop): stream_client(evloop) {
	initTimer();
	sc = this;
}

SimpleClient::~SimpleClient() {

}

void SimpleClient::on_connect(tcpconn* conn) {
	outmessage msg;
	msg.begin(cmd_peer_login);
	msg.write_int((int)getpid());
	msg.end();
	conn_->sendmsg(&msg);
}

void SimpleClient::on_disconnect(tcpconn* conn) {
}

int SimpleClient::on_no_message(tcpconn*) {
	return 0;
}

int SimpleClient::on_message(inmessage* msg, tcpconn* conn, unsigned long ssid) {
	const short cmd = msg->command();
	switch (cmd) {
		case cmd_peer_login: {
			int retcode = msg->read_int();
			printf("login %s\n", retcode==0?"success":"failure");
			if (retcode == -1) exit(0);
			startTimer();
			break;
		}
		case cmd_echo: {
			std::string rsp = msg->read_cstring();
			printf("echo: %s\n", rsp.c_str());
			break;
		}
		case cmd_upper: {
			std::string rsp = msg->read_cstring();
			printf("upper: %s\n", rsp.c_str());
			break;
		}
		case cmd_chat2all: {
			int peerid = msg->read_int();
			std::string content = msg->read_cstring();
			printf("%d say: %s\n", peerid, content.c_str());
			break;
		}
	}
	return 0;
}
