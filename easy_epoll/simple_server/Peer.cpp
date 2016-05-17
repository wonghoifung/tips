#include "Peer.h"
#include "Message.h"
#include "StreamHandler.h"

Peer::Peer(const uint32_t peerid, StreamHandler* handler):peerid_(peerid),streamhandler_(handler) {

}

Peer::~Peer() {

}

void Peer::sendMsg(OutMessage* msg) {
	if (streamhandler_) {
		streamhandler_->Send(msg);
	}
}
