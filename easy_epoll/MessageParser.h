#ifndef MESSAGEPARSER_HEADER
#define MESSAGEPARSER_HEADER

#include <cstddef>

class InMessage;
class tcpconn;

class MessageParser
{
protected:
	MessageParser() {}
public:
	MessageParser(tcpconn* h):handler_(h) {}
	virtual ~MessageParser() {}
	virtual void reset(void) = 0;
	virtual int parse(const char* , const size_t ) = 0;
	static MessageParser* create(tcpconn* h);
protected:
	tcpconn* handler_;
};

#endif 

