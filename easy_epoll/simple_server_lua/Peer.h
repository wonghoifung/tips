#ifndef PEER_HEADER
#define PEER_HEADER

#include <stdint.h>

class tcpconn;
class outmessage;

class Peer 
{
	Peer(const Peer&);
	Peer& operator=(const Peer&);
	
public:
	Peer(const uint32_t peerid, tcpconn* handler);
	~Peer();
	const uint32_t getPeerId() const { return peerid_; }
	void setStreamHandler(tcpconn* handler) { streamhandler_ = handler; }
	tcpconn* getStreamHandler() { return streamhandler_; }
	void sendMsg(outmessage* msg);
private:
	const uint32_t peerid_;
	tcpconn* streamhandler_;
};

#endif
