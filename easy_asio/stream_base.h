#ifndef stream_base_header
#define stream_base_header

#include <boost/asio.hpp>
#include <string>
#include <list>

class shared_const_buffer;

class stream_base
{
public:
	stream_base(boost::asio::io_service& io_service);
	virtual ~stream_base(void);
	int send(const char* buf, const size_t len);
	void connect(const std::string& host, const std::string& port);
	void open();
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_connect( const boost::system::error_code& error);
	boost::asio::ip::tcp::socket& socket() { return socket_; }

protected:
	virtual int on_close(const int, std::string) = 0;
	virtual int on_rawdata(const char*, const size_t) = 0;
	virtual int on_connect(const int) = 0;

	void close();
	void prepare_and_read();
	void close_read();
	void handle_write(const boost::system::error_code& error, size_t /*bytes_transferred*/);
	void close_and_callback(const int err, std::string mess = "");
	void post_read_event();
	void post_write_event();

	boost::asio::ip::tcp::socket socket_;
	enum { BUFFER_SZIE = 1024 * 8 };
	char m_data[BUFFER_SZIE];
	int readpostcnt_;
	int writepostcnt_;
	bool delayclose_;
	std::list<shared_const_buffer*> buffer_;
};

#endif
