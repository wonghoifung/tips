#ifndef SERVER_TCPCONN_HEADER
#define SERVER_TCPCONN_HEADER

#include "tcpconn.h"
#include "message.h"
#include "message_parser.h"
#include <map>

class server_tcpconn:public tcpconn				
{
	server_tcpconn(const server_tcpconn&);
	server_tcpconn& operator=(const server_tcpconn&);
public:
	explicit server_tcpconn(int nID);
	virtual ~server_tcpconn(void);

	const int status(void) const { return status_; }

	const int connid(void) const { return connid_; }

	const std::string& remoteaddr(void) const { return remoteaddr_; }		
	
	void* getud() { return ud_; }	
	void setud(void* pUserData) { ud_ = pUserData; }

	int     sendmsg(outmessage* msg);
    virtual int on_message(inmessage *);

private:
	virtual int on_rawdata(char *buf, int nLen);
	virtual int on_close(void);
	virtual int on_connect(void);
    virtual int	on_timeout(int Timerid);
	void GetRemoteAddr(void);

	int status_;
	int connid_;
	std::string remoteaddr_;
	int port_;
	void* ud_;
    message_parser* parser_;
};

#endif
