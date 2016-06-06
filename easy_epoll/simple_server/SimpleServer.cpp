#include "SimpleServer.h"
#include "commands.h"
#include "message.h"
#include "Peer.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

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

SimpleServer::SimpleServer(eventloop* evloop):stream_server(evloop), svid_(0) {
	heartbeatTimer_.set_handler(this, HEARTBEAT_TIMER);
	updateTimer_.set_handler(this, UPDATE_TIMER);
}

SimpleServer::~SimpleServer() {

}

bool SimpleServer::init(int port) {
	// svid_...
	if (!stream_server::init(port)) return false;

	heartbeatTimer_.start(30);
	updateTimer_.start(6);
	return true;
}

int SimpleServer::on_message(inmessage* pMessage, tcpconn* conn, unsigned long ssid) {
	const short cmd = pMessage->command();

	switch (cmd) {
		case cmd_peer_login:
			return handlePeerLogin(pMessage, conn);
		case cmd_req_broadcast:
			return handleReqBroadcast(pMessage);
	}

	Peer* peer = getPeer(conn);
	if (peer == NULL) {
		return -1;
	}

	switch (cmd) {
		case cmd_echo:
			return handleEcho(peer, pMessage);
	}

	return 0;
}

void SimpleServer::on_connect(tcpconn* conn) {
	printf("%s connected\n", conn->remoteaddr().c_str());
}

void SimpleServer::on_disconnect(tcpconn* conn) {
	printf("%s disconnected\n", conn->remoteaddr().c_str());
	Peer* peer = getPeer(conn);
	if (peer) {
		removePeer(peer);
	}
}

int SimpleServer::on_no_message(tcpconn*) {
	return 0;
}

int SimpleServer::on_timeout(int timerid) {
	switch (timerid) {
		case HEARTBEAT_TIMER: {
			printf("heartbeat timer, %d\n", (int)time(NULL));
			heartbeatTimer_.start(30);
			break;
		}

		case UPDATE_TIMER: {
			// update status or cache...
			printf("update timer, %d\n", (int)time(NULL));
			updateTimer_.start(6);
			break;
		}
	}
	return 0;
}

Peer* SimpleServer::getPeer(tcpconn* conn) {
	Peer* peer = NULL;
	if (conn) {
		void* ptr = conn->getud();
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
	peer->getStreamHandler()->setud(NULL);
	delete peer;
}

Peer* SimpleServer::checkRelogin(const uint32_t peerid, tcpconn* conn) {
	Peer* peer = ::getPeer(peerid);
	if (peer) {
		int reason = 0; // relogin

		outmessage msg;
		msg.begin(cmd_kick);
		msg.write_int(peerid);
		msg.write_int(reason);
		msg.end();
		peer->sendMsg(&msg);

		tcpconn* oldhandler = peer->getStreamHandler();
		if (oldhandler == conn) {
			printf("tricky thing happened\n");
			return peer;
		}

		removePeer(peer);

		if (oldhandler) {
			evloop()->close_conn(oldhandler);
		}

		return NULL;
	}
	return peer;
}

Peer* SimpleServer::newPeer(uint32_t peerid, tcpconn* conn) {
	Peer* peer = new Peer(peerid, conn);
	if (peer) {
		// init peer...

		::addPeer(peer);
		conn->setud(peer);
	}
	return peer;
}

int SimpleServer::handlePeerLogin(inmessage* message, tcpconn* conn) {
	if (conn->getud()) {
		printf("not a new connection\n");
		return -1;
	}
	const uint32_t peerid = message->read_int();
	Peer* peer = checkRelogin(peerid, conn);
	if (peer == NULL) {
		peer = newPeer(peerid, conn);
		if (peer == NULL) {
			outmessage msg;
			msg.begin(cmd_peer_login);
			msg.write_int(-1); // failure
			msg.end();
			conn->sendmsg(&msg);
			return -1;
		}
	} else {
		return -1;
	}

	outmessage msg;
	msg.begin(cmd_peer_login);
	msg.write_int(0); // success
	msg.end();
	conn->sendmsg(&msg);
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
