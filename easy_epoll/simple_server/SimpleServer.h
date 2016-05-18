#ifndef SIMPLESERVER_HEADER
#define SIMPLESERVER_HEADER

#include "StreamServer.h"
#include "TimerEvent.h"
#include "TimerOutEvent.h"

class Peer;

class SimpleServer : public StreamServer, public TimerOutEvent
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
	virtual int ProcessOnTimerOut(int timerid);
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
    TimerEvent heartbeatTimer_;
	TimerEvent updateTimer_;
};

#endif
