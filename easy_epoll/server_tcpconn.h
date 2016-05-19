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

	int GetChunkStatus(void){return m_nStatus;}

	int GetHandlerID(void){return m_nHandlerID;}

	std::string GetAddr(void){return m_addrremote;}		
	
	void *  GetUserData(){return m_pUserData;}	
	void    SetUserData(void *pUserData){	m_pUserData = pUserData;}

	int     sendmsg(outmessage* msg);
    virtual int on_message(inmessage *);

private:
	virtual int on_rawdata(char *buf, int nLen);
	virtual int on_close(void);
	virtual int on_connect(void);
    virtual int	on_timeout(int Timerid);
	void GetRemoteAddr(void);

	int    m_nStatus;
	int    m_nHandlerID;
	std::string m_addrremote;
	int    m_nPort;
	void * m_pUserData;
    message_parser* m_pParser;
};

#endif
