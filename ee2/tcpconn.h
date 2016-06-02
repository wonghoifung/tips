#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

class inmessage;

class tcpconn
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);
public:
	tcpconn();
	~tcpconn();
	int process_message(inmessage* imsg);
};

#endif
