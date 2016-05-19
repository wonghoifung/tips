#ifndef PEER_HEADER
#define PEER_HEADER

#include <stdint.h>

class StreamHandler;
class outmessage;

class Peer 
{
	Peer(const Peer&);
	Peer& operator=(const Peer&);
	
public:
	Peer(const uint32_t peerid, StreamHandler* handler);
	~Peer();
	const uint32_t getPeerId() const { return peerid_; }
	void setStreamHandler(StreamHandler* handler) { streamhandler_ = handler; }
	StreamHandler* getStreamHandler() { return streamhandler_; }
	void sendMsg(outmessage* msg);
private:
	const uint32_t peerid_;
	StreamHandler* streamhandler_;
};

#endif
