#ifndef MESSAGEPARSER_HEADER
#define MESSAGEPARSER_HEADER

#include <cstddef>

class InMessage;
class TcpHandler;

class MessageParser
{
protected:
	MessageParser() {}
public:
	MessageParser(TcpHandler* h):handler_(h) {}
	virtual ~MessageParser() {}
	virtual void reset(void) = 0;
	virtual int parse(const char* , const size_t ) = 0;
	static MessageParser* create(TcpHandler* h);
protected:
	TcpHandler* handler_;
};

#endif 

