#ifndef SIMPLECLIENT_HEADER
#define SIMPLECLIENT_HEADER

#include "stream_server.h"
#include "stream_client.h"
#include "timer.h"

class SimpleClient : public stream_server
{
	SimpleClient(const SimpleClient&);
	SimpleClient& operator=(const SimpleClient&);
public:
	SimpleClient();
	~SimpleClient();
	bool init();
	bool connect(const std::string& host, const std::string& port);
 	int send(outmessage* msg);
    void handle_connect(tcpconn* conn);
    void handle_disconnect(tcpconn* conn);
	int handle_timeout(tcpconn*);
	int handle_message(inmessage* msg, tcpconn* conn, unsigned long ssid);
private:
	stream_client connector_;
	tcpconn* conn_;
};

#endif
