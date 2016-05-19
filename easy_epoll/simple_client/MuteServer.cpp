#include "MuteServer.h"
#include <stdio.h>

MuteServer::MuteServer() {

}

MuteServer::~MuteServer() {

}

bool MuteServer::init() {
	return true;
}

int MuteServer::ProcessMessage(inmessage* pMessage, StreamHandler* pHandler, unsigned long dwSessionID) {
	printf("MuteServer::ProcessMessage\n");
	return 0;
}

void MuteServer::OnConnect(StreamHandler* pHandler) {
	printf("MuteServer::OnConnect\n");
}

void MuteServer::OnDisconnect(StreamHandler* pHandler) {
	printf("MuteServer::OnDisconnect\n");
}
