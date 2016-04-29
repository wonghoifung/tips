#include "stream_handler.h"
#include "log.h"

#include <boost/bind.hpp>

stream_handler::stream_handler(boost::asio::io_service& io_service, const unsigned int hcnt, message_handler* handler, bool alarm)
	: stream_base(io_service), timer_(io_service), msgparser_(NULL), msghandler_(handler),
	ud_(NULL), state_(-1), alarm_enable_(alarm), eventmask_(stream_handler::ENABLE_ALL_EVENT)
{

}

stream_handler::~stream_handler(void)
{
	disable_alarm();
 	if (msgparser_ != NULL)	{
 		delete msgparser_;
 		msgparser_ = NULL;
 	}
}

void stream_handler::disable_alarm()
{
	boost::system::error_code ec;
	timer_.cancel(ec);
}

int stream_handler::send(outmessage *msg)
{
	return stream_base::send(msg->packet_buf(), msg->packet_size());
}

int stream_handler::on_rawdata(const char * buf, const size_t len)
{
	if (msgparser_ == NULL)	{
		msgparser_ = message_parser::create(this);
	}
	return msgparser_->parse(buf, len);
}

int stream_handler::on_close(const int err, std::string mess)
{
	if (state_ != CLOSE)
	{
		state_ = CLOSE;
		if (eventmask_ & stream_handler::ENABLE_CLOSE_EVENT) {
			return msghandler_->on_close(this, err);
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

int stream_handler::on_connect(const int e)
{
	if (e == 0) {
		state_ = CONNECT;
		enable_alarm();
		return msghandler_->on_connect(this);
	}
	else {
		return e;
	}
}

void stream_handler::enable_alarm()
{
	if (alarm_enable_) {
		timer_.expires_from_now(boost::posix_time::seconds(15));
		timer_.async_wait(boost::bind(&stream_handler::on_alarm, this, boost::asio::placeholders::error));
	}
}

void stream_handler::on_alarm(const boost::system::error_code & error)
{
	if (error.value() == 0)	{
		close();
	}
}

void stream_handler::close(bool disablecloseevent)
{
	if (disablecloseevent) {
		eventmask_ = (eventmask_ & (~stream_handler::ENABLE_CLOSE_EVENT));
	}
	if(state_ != CLOSE) {
		stream_base::close();
	}
}

int stream_handler::on_parse_complete(inmessage* msg)
{
	if (state_ == CONNECT) {
		state_ = REQUEST;
		disable_alarm();
	}
	return msghandler_->on_message(this, msg);
}

int stream_handler::set_sndbufsize(int send_buffer_size, bool force)
{
	boost::system::error_code ec;
	if (force) {
		boost::asio::socket_base::send_buffer_size op(send_buffer_size );
		socket().set_option(op, ec);
	}
	else {
		boost::asio::socket_base::send_buffer_size sbs;
		socket().get_option(sbs, ec);
		if (ec.value() != 0 || sbs.value() < send_buffer_size) {
			boost::asio::socket_base::send_buffer_size op(send_buffer_size );
			socket().set_option(op, ec);
		}
	}
	return ec.value();
}

int stream_handler::set_rcvbufsize(int receive_buffer_size, bool force)
{
	boost::system::error_code ec;
	if (force) {
		boost::asio::socket_base::receive_buffer_size rs(receive_buffer_size);
		socket().set_option(rs, ec);
	}
	else {
		boost::asio::socket_base::receive_buffer_size rbs;
		socket().get_option(rbs, ec);
		if (ec.value() != 0 || rbs.value() < receive_buffer_size) {
			boost::asio::socket_base::receive_buffer_size  rs(receive_buffer_size);
			socket().set_option(rs, ec);
		}
	}
	return ec.value();
}
