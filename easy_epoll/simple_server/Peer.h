#ifndef PEER_HEADER
#define PEER_HEADER

#include <stdint.h>

class server_tcpconn;
class outmessage;

class Peer 
{
	Peer(const Peer&);
	Peer& operator=(const Peer&);
	
public:
	Peer(const uint32_t peerid, server_tcpconn* handler);
	~Peer();
	const uint32_t getPeerId() const { return peerid_; }
	void setStreamHandler(server_tcpconn* handler) { streamhandler_ = handler; }
	server_tcpconn* getStreamHandler() { return streamhandler_; }
	void sendMsg(outmessage* msg);
private:
	const uint32_t peerid_;
	server_tcpconn* streamhandler_;
};

#endif
