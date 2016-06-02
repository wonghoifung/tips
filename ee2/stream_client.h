#ifndef STREAM_CLIENT_HEADER
#define STREAM_CLIENT_HEADER

#include "ae.h"

class stream_client
{
	stream_client(const stream_client&);
	stream_client& operator=(const stream_client&);
public:
	stream_client(aeEventLoop* evloop);
	~stream_client();
private:
	aeEventLoop* evloop_;
};

#endif
