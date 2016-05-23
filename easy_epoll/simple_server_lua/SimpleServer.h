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
	int lua_on_message(inmessage* pMessage, tcpconn* pHandler, unsigned long dwSessionID);
	virtual int on_message(inmessage* pMessage, tcpconn* pHandler, unsigned long dwSessionID);
    virtual void on_connect(tcpconn* pHandler);
    virtual void on_disconnect(tcpconn* pHandler);
    virtual int on_no_message(tcpconn*);
	virtual int on_timeout(int timerid);
protected:
	Peer* getPeer(tcpconn* pHandler);
	int removePeer(Peer* peer);
	void delPeer(Peer* peer);
	Peer* checkRelogin(const uint32_t peerid, tcpconn* pHandler);
	Peer* newPeer(uint32_t peerid, tcpconn* pHandler);
	int handlePeerLogin(inmessage* message, tcpconn* pHandler);
	int handleReqBroadcast(inmessage* message);
	int handleEcho(Peer* peer, inmessage* message);
private:
	uint32_t svid_;
    timer heartbeatTimer_;
	timer updateTimer_;
};

#endif

