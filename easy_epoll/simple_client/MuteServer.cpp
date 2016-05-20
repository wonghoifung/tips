#include "MuteServer.h"
#include <stdio.h>

MuteServer::MuteServer() {

}

MuteServer::~MuteServer() {

}

bool MuteServer::init() {
	return true;
}

int MuteServer::handle_message(inmessage* pMessage, tcpconn* conn, unsigned long dwSessionID) {
	printf("MuteServer::handle_message\n");
	return 0;
}

void MuteServer::OnConnect(tcpconn* conn) {
	printf("MuteServer::OnConnect\n");
}

void MuteServer::OnDisconnect(tcpconn* conn) {
	printf("MuteServer::OnDisconnect\n");
}
