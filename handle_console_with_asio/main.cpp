#include "console_handler.h"

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		console_handler c(io_service);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}

