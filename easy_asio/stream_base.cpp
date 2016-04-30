#include "stream_base.h"
#include "shared_const_buffer.h"
#include <boost/bind.hpp>

static int atoi_s(const std::string& str) { return str.empty() ? 0 : atoi(str.c_str()); }

stream_base::stream_base(boost::asio::io_service& io_service)
	: socket_(io_service), readpostcnt_(0), writepostcnt_(0), delayclose_(false)
{

}

stream_base::~stream_base( void )
{
	for (std::list<shared_const_buffer*>::iterator it = buffer_.begin(); it != buffer_.end(); ++it)
	{
		delete *it;
	}
  	buffer_.clear();
}

void stream_base::handle_write( const boost::system::error_code & error, size_t bytes_transferred)
{
	--writepostcnt_;
	assert(writepostcnt_ >= 0 && readpostcnt_ >= 0);
	if (error.value() != 0) {
		invoke_close_callback_and_self_delete(error.value(),error.message());
	}
	else {
		shared_const_buffer* p = buffer_.front();
		p->read(bytes_transferred);
		if (p->size() == 0) {
			buffer_.pop_front();
			delete p;
			p = NULL;
		}

		if (!buffer_.empty()) {
 			post_write_event();
		}
		else if (delayclose_) {
			if (writepostcnt_ == 0 && readpostcnt_ == 0) {
				invoke_close_callback_and_self_delete(0);
			}
			else if (writepostcnt_ == 0) {
				boost::system::error_code e;
				socket_.close(e); // initiate to close, there is pending read, therefore callback cleanup will happen in next loop
			}
		}
	}
}

void stream_base::invoke_close_callback_and_self_delete(const int err, std::string mess)
{
	on_close(err, mess);
	if (writepostcnt_ == 0 && readpostcnt_ == 0) {
		delete this;
	}
}

int stream_base::send(const char * buf, const size_t len)
{
	if (delayclose_ == false) {
		buffer_.push_back(new shared_const_buffer(buf, len));
		if (writepostcnt_ == 0) post_write_event();
	}
	return 0;
}

void stream_base::prepare_and_read()
{
	boost::system::error_code e;

	// boost::asio::socket_base::keep_alive op(true);
	// socket().set_option(op, e);

	// {
	// 	typedef boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPIDLE> keep_aliveidle;
	// 	socket().set_option(keep_aliveidle(60), e);
	// }

	// {
	// 	typedef boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPINTVL> keep_aliveintvl;
	// 	socket().set_option(keep_aliveintvl(30), e);
	// }

	// {
	// 	typedef boost::asio::detail::socket_option::integer<IPPROTO_TCP, TCP_KEEPCNT> keep_alivecnt;
	// 	socket().set_option(keep_alivecnt(3), e);
	// }

	boost::asio::ip::tcp::no_delay option(true);
	socket().set_option(option, e);

	post_read_event();
}

void stream_base::post_read_event()
{
	socket_.async_receive(boost::asio::buffer(m_data, BUFFER_SZIE),
						boost::bind(&stream_base::handle_read, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	++readpostcnt_;
}

void stream_base::post_write_event()
{
	socket_.async_send(boost::asio::buffer(buffer_.front()->get() , buffer_.front()->size()),
						boost::bind(&stream_base::handle_write, this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred));
	++writepostcnt_;
}

void stream_base::open()
{
	if(on_connect(0) != 0) return;
	prepare_and_read();
}

void stream_base::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	--readpostcnt_;
	if (error.value() == 0 && delayclose_ == false) {
		on_rawdata(m_data, bytes_transferred);
		post_read_event();
	} else {
		invoke_close_callback_and_self_delete(error.value(), error.message());
	}
}

void stream_base::close()
{
	if (writepostcnt_ > 0) {
		close_read();
	} else {
		boost::system::error_code e;
		socket_.close(e); // initiate to close, there is pending read, therefore callback cleanup will happen in next loop
	}
}

void stream_base::connect(const std::string& host, const std::string& port)
{
	socket_.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host),
		static_cast<unsigned short>(atoi_s(port))),
		boost::bind(&stream_base::handle_connect, this, boost::asio::placeholders::error));
}

void stream_base::handle_connect( const boost::system::error_code& error )
{
	prepare_and_read();
	on_connect(error.value());
}

void stream_base::close_read()
{
	delayclose_ = true;
	boost::system::error_code e;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, e);
}
