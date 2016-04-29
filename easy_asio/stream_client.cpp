#include "stream_client.h"

stream_client::stream_client() : timer_(NULL), io_service_(NULL), handler_(NULL)
{

}

stream_client::~stream_client()
{
  if (timer_ != NULL)
	{
		boost::system::error_code e;
		timer_->cancel(e);
		delete timer_;
		timer_ = NULL;
	}
}

int stream_client::on_connect(stream_handler* handler)
{
  if (connecthandler_.empty() == false)
	{
		return connecthandler_(handler);
	}
	return 0;
}

int stream_client::on_message(stream_handler* handler, inmessage* inmsg)
{
  if (message_handler_.empty() == false)
	{
		return message_handler_(handler, inmsg);
	}
  return 0;
}

int stream_client::on_close(stream_handler* handler, const int err)
{
  handler_ = NULL;
  if (timer_ == NULL)
  {
    timer_ = new boost::asio::deadline_timer(*io_service_);
  }

  timer_->expires_from_now(boost::posix_time::seconds(30));
  timer_->async_wait(boost::bind(&stream_client::on_alarm, this, boost::asio::placeholders::error));

  if (closehandler_.empty() == false)
  {
    return closehandler_(handler);
  }
  return 0;
}

void stream_client::connect(boost::asio::io_service& io_service, const std::string& host, const std::string& port)
{
  host_ = host;
  port_ = port;
  io_service_ = &io_service;
  try_connect(host_, port_);
}

void stream_client::send(outmessage* outmsg)
{
  if (handler_ != NULL)
	{
		handler_->send(outmsg);
	}
}

void stream_client::try_connect(const std::string& host, const std::string& port)
{
  if (handler_ == NULL)
  {
    handler_ = new stream_handler(*io_service_, 0, this, false);
  }
  handler_->connect(host, port);
}

void stream_client::on_alarm(const boost::system::error_code& error)
{
  if (error.value() == 0)
	{
		try_connect(host_, port_);
	}
}
