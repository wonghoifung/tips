#ifndef STREAMHANDLER_HEADER
#define STREAMHANDLER_HEADER

#include "TcpHandler.h"
#include "Message.h"
#include "MessageParser.h"
#include <map>

class StreamHandler:public TcpHandler				
{
public:
	explicit StreamHandler(int nID);
	virtual ~StreamHandler(void);

	int GetChunkStatus(void){return m_nStatus;}

	int GetHandlerID(void){return m_nHandlerID;}

	std::string GetAddr(void){return m_addrremote;}		
	
	void *  GetUserData(){return m_pUserData;}	
	void    SetUserData(void *pUserData){	m_pUserData = pUserData;}

	int     Send(OutMessage *pPacket);
    virtual int OnParserComplete(InMessage *);

private:
	virtual int OnParser(char *buf, int nLen);
	virtual int OnClose(void);
	virtual int OnConnected(void);
    virtual int	ProcessOnTimerOut(int Timerid);
	void GetRemoteAddr(void);

	int    m_nStatus;
	int    m_nHandlerID;
	std::string m_addrremote;
	int    m_nPort;
	void * m_pUserData;
    MessageParser * m_pParser;
};

#endif
