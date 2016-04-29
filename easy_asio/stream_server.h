#ifndef stream_server_header
#define stream_server_header

#include <string>
#include <boost/asio.hpp>

class message_handler;
class stream_handler;

class stream_server
{
public:
	stream_server(boost::asio::io_service& io_service, const std::string& host, const short port, message_handler* msghandler);
	~stream_server() {}
	void handle_accept(stream_handler* sh, const boost::system::error_code& error);
	void run() { io_service_.run(); }

private:
	void post_accept_event();

private:
	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::acceptor acceptor_;
	message_handler* msghandler_;
	unsigned int hcnt_;
};

#endif
