#include "stream_server.h"
#include "stream_handler.h"
#include "message_handler.h"
#include "stream_base.h"
#include <boost/bind.hpp>

stream_server::stream_server(boost::asio::io_service& io_service, const std::string& host, short port, message_handler* msghandler)
	:io_service_(io_service),
	acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(host), port)), 
	msghandler_(msghandler), 
	hcnt_(0)
{
	for (size_t i = 0; i < 100; ++i) {
		post_accept_event();
	}
}

void stream_server::handle_accept(stream_handler* sh, const boost::system::error_code& error)
{
	if (error.value() == 0) {
		sh->open();
	}
	else {
		delete sh;
		sh = NULL;
	}
	post_accept_event();
}

void stream_server::post_accept_event()
{
	stream_handler* sh = new stream_handler(io_service_, ++hcnt_, msghandler_);
	if (sh != NULL) {
		acceptor_.async_accept(sh->socket(),
			boost::bind(&stream_server::handle_accept, this, sh,
			boost::asio::placeholders::error));
	}
}
