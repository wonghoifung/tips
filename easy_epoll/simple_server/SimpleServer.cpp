#include "SimpleServer.h"
#include "commands.h"
#include "Peer.h"
#include <assert.h>
#include <stdio.h>

enum { HEARTBEAT_TIMER = 1, UPDATE_TIMER };

typedef std::map<uint32_t, Peer*> pmap;
pmap peers;
static bool exist(uint32_t peerid) {
	pmap::iterator it = peers.find(peerid);
	return it != peers.end();
}
static Peer* getPeer(uint32_t peerid) {
	pmap::iterator it = peers.find(peerid);
	if (it != peers.end()) return it->second;
	return NULL;
}
static bool addPeer(Peer* peer) {
	if (exist(peer->getPeerId())) {
		assert(false);
		return false;
	}
	peers[peer->getPeerId()] = peer;
	return true;
}
static bool delPeer(uint32_t peerid) {
	pmap::iterator it = peers.find(peerid);
	if (it != peers.end()) {
		peers.erase(it);
		return true;
	}
	return false;
}
static void broadcast(outmessage* msg) {
	pmap::iterator it = peers.begin();
	for (; it != peers.end(); ++it) {
		it->second->sendMsg(msg);
	}
}

const std::string sys_key("1234567890");

SimpleServer::SimpleServer():svid_(0) {
	heartbeatTimer_.set_handler(this, HEARTBEAT_TIMER);
	updateTimer_.set_handler(this, UPDATE_TIMER);
}

SimpleServer::~SimpleServer() {

}

bool SimpleServer::init() {
	// svid_...

	heartbeatTimer_.start(30);
	updateTimer_.start(6);
	return true;
}

int SimpleServer::ProcessMessage(inmessage* pMessage, server_tcpconn* pHandler, unsigned long dwSessionID) {
	const short cmd = pMessage->command();

	switch (cmd) {
		case cmd_peer_login:
			return handlePeerLogin(pMessage, pHandler);
		case cmd_req_broadcast:
			return handleReqBroadcast(pMessage);
	}

	Peer* peer = getPeer(pHandler);
	if (peer == NULL) {
		return -1;
	}

	switch (cmd) {
		case cmd_echo:
			return handleEcho(peer, pMessage);
	}

	return 0;
}

void SimpleServer::OnConnect(server_tcpconn* pHandler) {

}

void SimpleServer::OnDisconnect(server_tcpconn* pHandler) {
	Peer* peer = getPeer(pHandler);
	if (peer) {
		removePeer(peer);
	}
}

int SimpleServer::on_timeout(int timerid) {
	switch (timerid) {
		case HEARTBEAT_TIMER: {
			printf("heartbeat timer\n");
			heartbeatTimer_.start(30);
			break;
		}

		case UPDATE_TIMER: {
			// update status or cache...
			printf("update timer\n");
			updateTimer_.start(6);
			break;
		}
	}
	return 0;
}

Peer* SimpleServer::getPeer(server_tcpconn* pHandler) {
	Peer* peer = NULL;
	if (pHandler) {
		void* ptr = pHandler->GetUserData();
		if (ptr) peer = reinterpret_cast<Peer*>(ptr);
	}
	return peer;
}

int SimpleServer::removePeer(Peer* peer) {
	// to notify all modules...
	delPeer(peer);
	return 0;
}

void SimpleServer::delPeer(Peer* peer) {
	::delPeer(peer->getPeerId());
	peer->getStreamHandler()->SetUserData(NULL);
	delete peer;
}

Peer* SimpleServer::checkRelogin(const uint32_t peerid, server_tcpconn* pHandler) {
	Peer* peer = ::getPeer(peerid);
	if (peer) {
		int reason = 0; // relogin

		outmessage msg;
		msg.begin(cmd_kick);
		msg.write_int(peerid);
		msg.write_int(reason);
		msg.end();
		peer->sendMsg(&msg);

		server_tcpconn* oldhandler = peer->getStreamHandler();
		if (oldhandler == pHandler) {
			printf("tricky thing happened\n");
			return peer;
		}

		removePeer(peer);

		if (oldhandler) {
			disconnect(oldhandler);
		}

		return NULL;
	}
	return peer;
}

Peer* SimpleServer::newPeer(uint32_t peerid, server_tcpconn* pHandler) {
	Peer* peer = new Peer(peerid, pHandler);
	if (peer) {
		// init peer...

		::addPeer(peer);
		pHandler->SetUserData(peer);
	}
	return peer;
}

int SimpleServer::handlePeerLogin(inmessage* message, server_tcpconn* pHandler) {
	if (pHandler->GetUserData()) {
		printf("not a new connection\n");
		return -1;
	}
	const uint32_t peerid = message->read_int();
	Peer* peer = checkRelogin(peerid, pHandler);
	if (peer == NULL) {
		peer = newPeer(peerid, pHandler);
		if (peer == NULL) {
			outmessage msg;
			msg.begin(cmd_peer_login);
			msg.write_int(-1); // failure
			msg.end();
			pHandler->sendmsg(&msg);
			return -1;
		}
	} else {
		return -1;
	}

	outmessage msg;
	msg.begin(cmd_peer_login);
	msg.write_int(0); // success
	msg.end();
	pHandler->sendmsg(&msg);
	return 0;
}

int SimpleServer::handleReqBroadcast(inmessage* message) {
	if (sys_key == message->read_cstring()) {
		const int type = message->read_int();
		const std::string content = message->read_cstring();
		outmessage msg;
		msg.begin(cmd_broadcast);
		msg.write_int(type);
		msg.write_string(content);
		msg.end();
		broadcast(&msg);
	}
	return 0;
}

int SimpleServer::handleEcho(Peer* peer, inmessage* message) {
	const std::string content = message->read_cstring();
	printf("receive: %s\n", content.c_str());
	outmessage msg;
	msg.begin(cmd_echo);
	msg.write_string(content);
	msg.end();
	peer->sendMsg(&msg);
	return 0;
}
