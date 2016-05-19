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
	virtual int ProcessMessage(inmessage* pMessage, server_tcpconn* pHandler, unsigned long dwSessionID);
    virtual void OnConnect(server_tcpconn* pHandler);
    virtual void OnDisconnect(server_tcpconn* pHandler);
	virtual int on_timeout(int timerid);
protected:
	Peer* getPeer(server_tcpconn* pHandler);
	int removePeer(Peer* peer);
	void delPeer(Peer* peer);
	Peer* checkRelogin(const uint32_t peerid, server_tcpconn* pHandler);
	Peer* newPeer(uint32_t peerid, server_tcpconn* pHandler);
	int handlePeerLogin(inmessage* message, server_tcpconn* pHandler);
	int handleReqBroadcast(inmessage* message);
	int handleEcho(Peer* peer, inmessage* message);
private:
	uint32_t svid_;
    timer heartbeatTimer_;
	timer updateTimer_;
};

#endif

