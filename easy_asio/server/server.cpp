#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <string.h>

#define mysock_bufsize 2048
struct mysock {
	mysock(boost::asio::io_service& ios):sock(ios) {}
	boost::asio::ip::tcp::socket sock;
	char buff[mysock_bufsize];

	void post_read_event()
	{
		std::cout<<"["<<__FUNCTION__<<"]"<<std::endl;
		memset(buff,0,mysock_bufsize);
		sock.async_receive(boost::asio::buffer(buff, mysock_bufsize),
							boost::bind(&mysock::handle_read, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
	}

	void post_write_event(char* buf, size_t len)
	{
		std::cout<<"["<<__FUNCTION__<<"]"<<std::endl;
		sock.async_send(boost::asio::buffer(buf,len),
							boost::bind(&mysock::handle_write, this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred));
	}

	void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
	{
		std::cout<<"["<<__FUNCTION__<<"] error:"<<error.value()<<std::endl;
		if (error.value() == 0) {
			std::cout<<buff<<std::endl;
			post_write_event(buff,bytes_transferred);
			//post_read_event();
		} else {
			sock.close();
			delete this;
		}
	}

	void handle_write(const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		std::cout<<"["<<__FUNCTION__<<"] error:"<<e.value()<<std::endl;
		if (e.value() != 0)
		{
			sock.close();
			delete this;
		} else {
			post_read_event();
		}
	}
};

boost::asio::io_service io_service;
boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 9999));

void post_accept_event();

void handle_accept(mysock* s, const boost::system::error_code& error)
{
	std::cout<<"["<<__FUNCTION__<<"] error:"<<error.value()<<std::endl;
	if (error.value() == 0) {
		s->post_read_event();

		std::cout<<"initiate to close sock"<<std::endl;
		s->sock.cancel();
		s->sock.close();
		std::cout<<"still current loop"<<std::endl;
	}
	else {
		delete s;
		s = NULL;
	}
	post_accept_event();
}

void post_accept_event() {
	std::cout<<"["<<__FUNCTION__<<"]"<<std::endl;
	mysock* s = new mysock(io_service);
	acceptor.async_accept(s->sock,
			boost::bind(&handle_accept, s,
			boost::asio::placeholders::error));
}
int main() {
	post_accept_event();
	io_service.run();
}