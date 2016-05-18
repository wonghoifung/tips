#ifndef SIMPLECLIENT_HEADER
#define SIMPLECLIENT_HEADER

#include "tcpconn.h"
#include "MessageParser.h"
#include "StreamServer.h"
#include "Connector.h"

class SimpleClient: public tcpconn
{
	SimpleClient(const SimpleClient&);
	SimpleClient& operator=(const SimpleClient&);

public:
	SimpleClient(StreamServer* ss);
	~SimpleClient();
	bool connect(const std::string& host, const std::string& port);
 	int send(OutMessage* msg);

	virtual int OnParserComplete(InMessage *);
	virtual int OnParser(char *buf, int nLen);
	virtual int OnClose(void);
	virtual int OnConnected(void);
    virtual int	ProcessOnTimerOut(int Timerid);

private:
	Connector connector_;
	MessageParser* parser_;
	StreamServer* ss_;
};

#endif
