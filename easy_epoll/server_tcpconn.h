#ifndef SERVER_TCPCONN_HEADER
#define SERVER_TCPCONN_HEADER

#include "tcpconn.h"
#include "message.h"
#include "message_parser.h"
#include <map>

class server_tcpconn:public tcpconn				
{
public:
	explicit server_tcpconn(int nID);
	virtual ~server_tcpconn(void);

	int GetChunkStatus(void){return m_nStatus;}

	int GetHandlerID(void){return m_nHandlerID;}

	std::string GetAddr(void){return m_addrremote;}		
	
	void *  GetUserData(){return m_pUserData;}	
	void    SetUserData(void *pUserData){	m_pUserData = pUserData;}

	int     Send(outmessage *pPacket);
    virtual int OnParserComplete(inmessage *);

private:
	virtual int OnParser(char *buf, int nLen);
	virtual int OnClose(void);
	virtual int OnConnected(void);
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
