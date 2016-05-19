#include "MuteServer.h"
#include <stdio.h>

MuteServer::MuteServer() {

}

MuteServer::~MuteServer() {

}

bool MuteServer::init() {
	return true;
}

int MuteServer::ProcessMessage(inmessage* pMessage, server_tcpconn* pHandler, unsigned long dwSessionID) {
	printf("MuteServer::ProcessMessage\n");
	return 0;
}

void MuteServer::OnConnect(server_tcpconn* pHandler) {
	printf("MuteServer::OnConnect\n");
}

void MuteServer::OnDisconnect(server_tcpconn* pHandler) {
	printf("MuteServer::OnDisconnect\n");
}
