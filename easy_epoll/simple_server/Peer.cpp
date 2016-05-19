#include "Peer.h"
#include "message.h"
#include "server_tcpconn.h"

Peer::Peer(const uint32_t peerid, server_tcpconn* handler):peerid_(peerid),streamhandler_(handler) {

}

Peer::~Peer() {

}

void Peer::sendMsg(outmessage* msg) {
	if (streamhandler_) {
		streamhandler_->sendmsg(msg);
	}
}
