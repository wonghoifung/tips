#ifndef message_handler_header
#define message_handler_header

class stream_handler;
class inmessage;

class message_handler
{
public:
	virtual ~message_handler() {};
	virtual int on_connect(stream_handler*) = 0;
	virtual int on_message(stream_handler*, inmessage*) = 0;
	virtual int on_close(stream_handler*, const int) = 0;
};

#endif
