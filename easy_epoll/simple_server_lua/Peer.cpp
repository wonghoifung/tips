#include "Peer.h"
#include "message.h"
#include "tcpconn.h"

Peer* getPeer(tcpconn* conn) {
	Peer* peer = NULL;
	if (conn) {
		void* ptr = conn->getud();
		if (ptr) peer = reinterpret_cast<Peer*>(ptr);
	}
	return peer;
}

Peer::Peer(const uint32_t peerid, tcpconn* handler):peerid_(peerid),streamhandler_(handler) {

}

Peer::~Peer() {

}

void Peer::sendMsg(outmessage* msg) {
	if (streamhandler_) {
		streamhandler_->sendmsg(msg);
	}
}
