#ifndef STREAMCLIENT_HEADER
#define STREAMCLIENT_HEADER

#include "event_loop.h"
#include "sockapi.h"

// TODO a client that can use server's eventloop

class stream_client : public event_handler
{
	stream_client(const stream_client&);
	stream_client& operator=(const stream_client&);

public:
	stream_client();
	virtual ~stream_client();

	// from event_loop
	virtual tcpconn* create_tcpconn(); 

	// bool init();
	bool connect(const std::string& host, const std::string& port);
	bool connect(tcpconn* conn, const std::string& strAddr, int port);
	bool connect(tcpconn* conn, const address& addr);
 	int send(outmessage* msg);

 	// from event_handler
    virtual void handle_connect_event(tcpconn* conn);
    virtual void handle_disconnect_event(tcpconn* conn);
	virtual int handle_timeout_event(tcpconn*);
	virtual int handle_message_event(inmessage* msg, tcpconn* conn, unsigned long ssid);

	// to subclass
	virtual void on_connect(tcpconn* conn) = 0;
    virtual void on_disconnect(tcpconn* conn) = 0;
	virtual int on_no_message(tcpconn* conn) = 0;
	virtual int on_message(inmessage* msg, tcpconn* conn, unsigned long ssid) = 0;

	// bool set_evloop(event_loop* pServer);
	// bool register_to_evloop(tcpconn* conn); // TODO
protected:
	tcpconn* conn_;
};

#endif
