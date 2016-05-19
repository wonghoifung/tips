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

	virtual int on_message(inmessage *);
	virtual int on_rawdata(char *buf, int nLen);
	virtual int on_close(void);
	virtual int on_connect(void);
    virtual int	on_timeout(int Timerid);

private:
	stream_client connector_;
	message_parser* parser_;
	stream_server* ss_;
};

#endif
