#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

#define RECV_BUFFER 4096

#include "timer.h"
#include "loopbuf.h"
#include <stdint.h>
#include <map>

class event_loop;
class inmessage;

const int MAX_LOOP_BUFFER_LEN = 64*1024;

#define RECV_BUFFER_SIZE (1024*32)
#define SEND_BUFFER_SIZE (1024*32)

class tcpconn : public timer_handler
{
public:
	tcpconn();	
	virtual ~tcpconn();	

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
    virtual int OnParserComplete(inmessage *)=0;
    bool Writable();
	event_loop* evloop() { return evloop_; }
	void evloop(event_loop* p) { evloop_ = p; }

protected:
	int m_sock_fd;
    uint32_t m_fd_index;             
	uint32_t m_SocketType;
    bool m_bNeedDel;
    bool m_bfull;
	timer m_TcpTimer;
	event_loop* evloop_;
	char m_pRecvBuffer[RECV_BUFFER_SIZE];	
	loopbuf* m_pSendLoopBuffer;
	char m_pTmpSendBuffer[SEND_BUFFER_SIZE];
};

#endif  


