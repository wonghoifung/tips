#include "core.h"
#include "stream_server.h"
#include <unistd.h>


int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;

	boost::asio::io_service io_service;

	core* processor = new core(1, io_service);
	stream_server server(io_service, "127.0.0.1", 9797, processor);

	return static_cast<int>(io_service.run());
}

