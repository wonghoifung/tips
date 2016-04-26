#ifndef console_handler_header
#define console_handler_header

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

namespace posix = boost::asio::posix;

static const char* prompt = ">>> ";
static const size_t prompt_len = strlen(prompt);

class console_handler : private boost::noncopyable
{
public:
	console_handler(boost::asio::io_service& io_service);
	~console_handler();

	void write(const char* buf, size_t len, bool hasdata);
	void start_read_input();

private:
	void handle_write_output(const boost::system::error_code& error);
	void handle_read_input(const boost::system::error_code& error, std::size_t length);
	void close();

	posix::stream_descriptor input_;
	posix::stream_descriptor output_;
	boost::asio::streambuf input_buffer_;
	char output_buffer_[1024];
};

#endif
