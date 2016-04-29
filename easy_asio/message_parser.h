#ifndef message_parser_header
#define message_parser_header

#include <cstddef>

class inmessage;

class message_parser_handler
{
public:
	virtual ~message_parser_handler() {};
	virtual int on_parse_complete(inmessage*) = 0;
};

class message_parser
{
protected:
	message_parser() {}
public:
	message_parser(message_parser_handler* h):handler_(h) {}
	virtual ~message_parser() {}
	virtual void reset(void) = 0;
	virtual int parse(const char* , const size_t ) = 0;
	static message_parser* create(message_parser_handler* h);
protected:
	message_parser_handler* handler_;
};

#endif
