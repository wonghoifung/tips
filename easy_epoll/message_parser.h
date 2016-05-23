#ifndef MESSAGE_PARSER_HEADER
#define MESSAGE_PARSER_HEADER

#include <cstddef>

class tcpconn;

class message_parser
{
protected:
	message_parser() {}
public:
	message_parser(tcpconn* h):conn_(h) {}
	virtual ~message_parser() {}
	virtual void reset(void) = 0;
	virtual int parse(const char* , const size_t ) = 0;
	static message_parser* create(tcpconn* h);
protected:
	tcpconn* conn_;
};

#endif 

