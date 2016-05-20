#ifndef SIMPLECLIENT_HEADER
#define SIMPLECLIENT_HEADER

#include "event_loop.h"
#include "timer.h"
#include "sockapi.h"

class SimpleClient : public event_handler
{
	SimpleClient(const SimpleClient&);
	SimpleClient& operator=(const SimpleClient&);
public:
	SimpleClient();
	~SimpleClient();
	virtual tcpconn* create_tcpconn(void);
	bool init();
	bool connect(const std::string& host, const std::string& port);
 	int send(outmessage* msg);
    virtual void handle_connect(tcpconn* conn);
    virtual void handle_disconnect(tcpconn* conn);
	virtual int handle_timeout(tcpconn*);
	virtual int handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid);

	bool set_evloop(event_loop* pServer);
	bool connect(tcpconn* conn, const std::string& strAddr, int port);
	bool connect(tcpconn* conn, const address& addr);
	bool register_to_evloop(tcpconn* conn);
private:
	tcpconn* conn_;
};

#endif
