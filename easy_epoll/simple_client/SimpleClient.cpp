#include "SimpleClient.h"
#include "sockapi.h"
#include "commands.h"
#include "message.h"
#include "tcpconn.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

static SimpleClient* sc = NULL;
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
	//conn_ = create_tcpconn();
	conn_ = new tcpconn(1);
	conn_->setneeddel(false);// TODO
}

SimpleClient::~SimpleClient() {
	if (conn_) {
		delete conn_;
		conn_ = NULL;
	}
}

tcpconn* SimpleClient::create_tcpconn(void) {
	return conn_;
}

bool SimpleClient::init() {
	return true;
}

bool SimpleClient::connect(const std::string& host, const std::string& port) {
	printf("SimpleClient::%s\n", __FUNCTION__);
	bool ret = connect(conn_, host, atoi(port.c_str()));
	if (!ret) {
		printf("connect error\n");
	}
	return ret;
}

int SimpleClient::send(outmessage* msg) {
	printf("SimpleClient::%s\n", __FUNCTION__);
	return conn_->sendbuf(msg->cbuffer(), msg->size());
}

void SimpleClient::handle_connect(tcpconn* conn) {
	printf("SimpleClient::%s\n", __FUNCTION__);
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

bool SimpleClient::set_evloop(event_loop* pServer) {

}

bool SimpleClient::connect(tcpconn* conn, const std::string& strAddr, int port) {
	// int sock_fd = socket_create();
	// if( conn == NULL || sock_fd < 0)
	// {	
	// 	return false;
	// }
	// if(socket_block_connect(sock_fd, strAddr.c_str(), port) == 0)
	// {	
 //        socket_buffer(sock_fd,16*1024);
 //        if(socket_nonblock(sock_fd) < 0)
 //        {
 //            printf("SetNonblock faild \n");
 //            socket_close(sock_fd);
 //            return false;
 //        }
 //        if(socket_keepalive(sock_fd) < 0)
 //        {
 //            printf("socket_keepalive faild \n");
 //            socket_close(sock_fd);
 //            return false;
 //        }
 //        conn->setfd(sock_fd);
	// 	return register_to_evloop(conn);			
	// }
	// socket_close(sock_fd);	
	// return false;
	return init_client(strAddr, port);
}

bool SimpleClient::connect(tcpconn* conn, const address& addr) {
	return connect(conn,addr.host,addr.port);
}

bool SimpleClient::register_to_evloop(tcpconn* conn) {
	return manage(conn);
}
