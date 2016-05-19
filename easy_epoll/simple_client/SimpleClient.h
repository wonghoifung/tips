#ifndef SIMPLECLIENT_HEADER
#define SIMPLECLIENT_HEADER

#include "tcpconn.h"
#include "message_parser.h"
#include "stream_server.h"
#include "stream_client.h"

class SimpleClient: public tcpconn
{
	SimpleClient(const SimpleClient&);
	SimpleClient& operator=(const SimpleClient&);

public:
	SimpleClient(stream_server* ss);
	~SimpleClient();
	bool connect(const std::string& host, const std::string& port);
 	int send(outmessage* msg);

	virtual int OnParserComplete(inmessage *);
	virtual int OnParser(char *buf, int nLen);
	virtual int OnClose(void);
	virtual int OnConnected(void);
    virtual int	ProcessOnTimerOut(int Timerid);

private:
	stream_client connector_;
	message_parser* parser_;
	stream_server* ss_;
};

#endif
