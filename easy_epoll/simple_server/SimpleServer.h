#ifndef SIMPLESERVER_HEADER
#define SIMPLESERVER_HEADER

#include "stream_server.h"
#include "timer.h"

class Peer;

class SimpleServer : public stream_server, public timer_handler
{
	SimpleServer(const SimpleServer&);
	SimpleServer& operator=(const SimpleServer&);
public:
	SimpleServer();
	~SimpleServer();
	bool init();
	virtual int ProcessMessage(InMessage* pMessage, StreamHandler* pHandler, unsigned long dwSessionID);
    virtual void OnConnect(StreamHandler* pHandler);
    virtual void OnDisconnect(StreamHandler* pHandler);
	virtual int on_timeout(int timerid);
protected:
	Peer* getPeer(StreamHandler* pHandler);
	int removePeer(Peer* peer);
	void delPeer(Peer* peer);
	Peer* checkRelogin(const uint32_t peerid, StreamHandler* pHandler);
	Peer* newPeer(uint32_t peerid, StreamHandler* pHandler);
	int handlePeerLogin(InMessage* message, StreamHandler* pHandler);
	int handleReqBroadcast(InMessage* message);
	int handleEcho(Peer* peer, InMessage* message);
private:
	uint32_t svid_;
    timer heartbeatTimer_;
	timer updateTimer_;
};

#endif

