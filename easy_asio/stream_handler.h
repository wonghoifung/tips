#ifndef stream_handler_header
#define stream_handler_header

#include "packet_base.h"
#include "packet_parser.h"
#include "message_handler.h"
#include "stream_base.h"

class stream_handler : public stream_base, public message_parser_handler
{
public:
	stream_handler(boost::asio::io_service& io_service, const unsigned int h, message_handler * handler, bool alarm = true);
	virtual ~stream_handler(void);

	const static unsigned int DISABLE_ALL_EVENT					= 0;
	const static unsigned int ENABLE_CLOSE_EVENT				= 1 << 0;
	const static unsigned int ENABLE_MESSAGE_EVENT			= 1 << 1;
	const static unsigned int ENABLE_CONNECT_EVENT		  = 1 << 2;
	const static unsigned int ENABLE_ALL_EVENT					= UINT_MAX;

	enum STATUS { CONNECT=0, REQUEST, CLOSE };

	unsigned int get_sockfd(void) { return static_cast<unsigned int>(socket_.native_handle()); }

	void* get_ud() { return ud_; };
	void set_ud(void* ud) { ud_ = ud; };

	int send(outmessage* msg);
	void close(bool disablecloseevent = false);

	int set_sndbufsize(int send_buffer_size, bool force = false);
	int set_rcvbufsize(int receive_buffer_size, bool force = false);

private:
	// required by stream_base
	virtual int on_rawdata(const char * buf, const size_t len);
	virtual int on_close(const int err,string mess="");
	virtual int on_connect(const int e);

	// required by message_parser_handler
	virtual int on_parse_complete(inmessage * pPack);

	void on_alarm(const boost::system::error_code& error);
	void enable_alarm();
	void disable_alarm();

private:
	boost::asio::deadline_timer timer_;
	message_parser* msgparser_;
	message_handler* msghandler_;
	void* ud_;
	int state_;
	bool alarm_enable_;
	unsigned int eventmask_;
};

#endif
