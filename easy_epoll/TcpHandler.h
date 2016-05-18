#ifndef TCPHANDLER_HEADER
#define TCPHANDLER_HEADER

#define RECV_BUFFER 4096

#include "timer.h"
#include "LoopBuffer.h"
#include <stdint.h>
#include <map>

class TcpServer;
class InMessage;

const int MAX_LOOP_BUFFER_LEN = 64*1024;

#define RECV_BUFFER_SIZE (1024*32)
#define SEND_BUFFER_SIZE (1024*32)

class TcpHandler : public timer_handler
{
public:
	TcpHandler();	
	virtual ~TcpHandler();	

	void SetFd(int sock_fd);
	int GetFd()const;

    uint32_t get_fd_index() { return m_fd_index;}
    void set_fd_index(uint32_t index) { m_fd_index = index;}
  
    bool GetNeedDel() { return m_bNeedDel; }
    void SetNeedDel(bool bDel) { m_bNeedDel = bDel; }

	int handle_OnConnected();
	int handle_read();
	int handle_output();
	int handle_close();

protected:
	virtual int OnClose(void) {return -1;}
	virtual int OnConnected(void) {return 0;}
	virtual int OnParser(char*, int) {return -1;}
	virtual int	on_timeout(int Timerid) {return 0;};	

public:	
	int Send(const char *buf, int nLen);
    virtual int OnParserComplete(InMessage *)=0;
    bool Writable();
	TcpServer * server(void){return m_pServer;}
	virtual void server(TcpServer *p){m_pServer = p;}

protected:
	int m_sock_fd;
    uint32_t m_fd_index;             
	uint32_t m_SocketType;
    bool m_bNeedDel;
    bool m_bfull;
	timer m_TcpTimer;
	TcpServer* m_pServer;
	char m_pRecvBuffer[RECV_BUFFER_SIZE];	
	LoopBuffer* m_pSendLoopBuffer;
	char m_pTmpSendBuffer[SEND_BUFFER_SIZE];
};

#endif  


