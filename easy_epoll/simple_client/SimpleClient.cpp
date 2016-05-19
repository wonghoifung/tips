#include "SimpleClient.h"
#include "SocketApi.h"
#include "commands.h"
#include <sys/types.h>
#include <unistd.h>

static SimpleClient* sc = NULL;
static timer gtimer;
static void startTimer() {
	gtimer.start(5);
}
static bool timerInit = false;
class tmp: public timer_handler {
public:
	int on_timeout(int tid) {
		OutMessage msg;
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

SimpleClient::SimpleClient(stream_server* ss):parser_(NULL),ss_(ss) {
	initTimer();
	sc = this;
	SetNeedDel(false);
}

SimpleClient::~SimpleClient() {
	if (parser_) {
		delete parser_;
		parser_ = NULL;
	}
}

bool SimpleClient::connect(const std::string& host, const std::string& port) {
	connector_.Open(ss_);
	bool ret = connector_.Connect(this, host, atoi(port.c_str()));
	if (!ret) {
		printf("connect error\n");
	}
	return ret;
}

int SimpleClient::send(OutMessage* msg) {
	return tcpconn::Send(msg->cbuffer(), msg->size());
}

int SimpleClient::OnParserComplete(inmessage* msg) {
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

int SimpleClient::OnParser(char *buf, int nLen) {
	if (parser_ == NULL) parser_ = MessageParser::create(this);
	return parser_->parse(buf, nLen);
}

int SimpleClient::OnClose(void) {
	printf("SimpleClient::OnClose\n");
	return 0;
}

int SimpleClient::OnConnected(void) {
	sockaddr_in remote_addr;
    memset(&remote_addr, 0, sizeof(remote_addr));
    int len = sizeof(remote_addr);
    if(getpeername(GetFd(), reinterpret_cast<sockaddr *> (&remote_addr), (socklen_t*)&len) == 0)
    {
        std::string remoteip = inet_ntoa(remote_addr.sin_addr);
        int remoteport = ntohs(remote_addr.sin_port);
        printf("%s:%d come in\n", remoteip.c_str(), remoteport);

        OutMessage msg;
		msg.begin(cmd_peer_login);
		msg.write_int((int)getpid());
		msg.end();
		send(&msg);
    }
    return 0;
}

int	SimpleClient::ProcessOnTimerOut(int Timerid) {
	printf("SimpleClient::ProcessOnTimerOut\n");
	return 0;
}


