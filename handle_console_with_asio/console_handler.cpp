#include "console_handler.h"

console_handler::console_handler(boost::asio::io_service& io_service)
	: input_(io_service, ::dup(STDIN_FILENO)),
	output_(io_service, ::dup(STDOUT_FILENO)),
	input_buffer_(1024)
{
	write(prompt, prompt_len, false);
}

console_handler::~console_handler()
{
	close();
}

void console_handler::write(const char* buf, size_t len, bool hasdata)
{
	if (hasdata) {
		static char eol[] = { '\n' };
		boost::array<boost::asio::const_buffer, 3> buffers = { { boost::asio::buffer(buf, len),
			boost::asio::buffer(eol), boost::asio::buffer(prompt, prompt_len) } };
		boost::asio::async_write(output_, buffers,
			boost::bind(&console_handler::handle_write_output, this,
			boost::asio::placeholders::error));
	}
	else {
		boost::array<boost::asio::const_buffer, 1> buffers = { { boost::asio::buffer(buf, len) } };
		boost::asio::async_write(output_, buffers,
			boost::bind(&console_handler::handle_write_output, this,
			boost::asio::placeholders::error));
	}
}

void console_handler::start_read_input()
{
	// Read a line of input entered by the user.
	boost::asio::async_read_until(input_, input_buffer_, '\n',
		boost::bind(&console_handler::handle_read_input, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void console_handler::handle_write_output(const boost::system::error_code& error)
{
	if (!error)
	{
		start_read_input();
	}
	else
	{
		close();
	}
}

void console_handler::handle_read_input(const boost::system::error_code& error, std::size_t length)
{
	if (!error)
	{
		// Write the message (minus the newline) to the server.
		memset(output_buffer_, 0, sizeof(output_buffer_));
		input_buffer_.sgetn(output_buffer_, length - 1);
		input_buffer_.consume(1); // Remove newline from input.

		if (strcmp(output_buffer_, "exit") == 0)
		{
			close();
			return;
		}

		if (strlen(output_buffer_) == 0) 
		{
			write(prompt, prompt_len, false);
			return;
		}

		write(output_buffer_, length - 1, true);
	}
	else if (error == boost::asio::error::not_found)
	{
		// Didn't get a newline. Send whatever we have.
		memset(output_buffer_, 0, sizeof(output_buffer_));
		input_buffer_.sgetn(output_buffer_, input_buffer_.size());

		if (strcmp(output_buffer_, "exit") == 0)
		{
			close();
			return;
		}

		if (strlen(output_buffer_) == 0)
		{
			write(prompt, prompt_len, false);
			return;
		}

		write(output_buffer_, input_buffer_.size(), true);
	}
	else
	{
		close();
	}
}

void console_handler::close()
{
	input_.close();
	output_.close();
}
