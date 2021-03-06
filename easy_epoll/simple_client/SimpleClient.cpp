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

		{
			outmessage msg;
			msg.begin(tblmsg_test1);
			msg.write_int(cc);
			msg.write_cstring("test1");
			msg.write_cstring("test11");
			msg.write_cstring("test111");
			msg.write_int(cc+1);
			msg.end();
			if (sc) sc->send(&msg);
		}

		{
			outmessage msg;
			msg.begin(tblmsg_test2);
			msg.write_cstring("test2");
			msg.write_int(cc);
			msg.write_int(cc+1);
			msg.write_int(cc+2);
			msg.write_cstring("test22");
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
		case tblmsg_test1: {
			// 1,2,2,2,1
			int i1 = msg->read_int();
			std::string s1 = msg->read_cstring();
			std::string s2 = msg->read_cstring();
			std::string s3 = msg->read_cstring();
			int i2 = msg->read_int();
			printf("tblmsg_test1: %d|%s|%s|%s|%d\n", i1, s1.c_str(), s2.c_str(), s3.c_str(), i2);
			break;
		}
		case tblmsg_test2: {
			// 2,1,1,1,2
			std::string s1 = msg->read_cstring();
			int i1 = msg->read_int();
			int i2 = msg->read_int();
			int i3 = msg->read_int();
			std::string s2 = msg->read_cstring();
			printf("tblmsg_test2: %s|%d|%d|%d|%s\n", s1.c_str(), i1, i2, i3, s2.c_str()); 
			break;
		}
	}
	return 0;
}
