#include "message.h"
#include "MessageParser.h"
#include "StreamHandler.h"

class concrete_message_parser :  public MessageParser
{
	enum {	state_header=0, state_body, state_done, state_error };

public:
	concrete_message_parser(tcpconn * h):MessageParser(h)
	{
		buf_ = inmsg_.buffer();
		reset();
	}

	virtual ~concrete_message_parser(void)
	{

	}

	int parse(const char* data, const size_t length)
	{
		size_t ndx = 0;
		while(ndx < length && state_ != state_error)
		{
			switch(state_)
			{
			case state_header:
				if(!read_header(data, length, ndx))
					break;
				if(parse_header() != 0) {
					state_ = state_error;
					break;
				} else {
					state_ = state_body;
				}
				// don't break, keep going

			case state_body:
				if(parse_body(data, length, ndx))
					state_ = state_done;
				break;

			default:
				return -1;
			}

			if(state_ == state_error) reset();

			if(state_ == state_done)
			{
				handler_->OnParserComplete(&inmsg_);
				reset();
			}
		}

		return 0;
	}

	virtual void reset(void)
	{
		state_ = state_header;
		pos_ = 0;
		bodylen_ = 0;
		inmsg_.reset();
	}

private:
	bool read_header(const char* data, const size_t length, size_t& ndx)
	{
		if (0 == ndx) pos_ = 0;

		while(pos_ < c_header_size && ndx < length) {
			buf_[pos_++] = data[ndx++];
		}

		if(pos_ < c_header_size)
			return false;

		return true;
	}

	int parse_header(void)
	{
		if(buf_[0] != 'G' || buf_[1] != 'P')
			return -1;

		bodylen_ = inmsg_.body_length();
		if(bodylen_ <= (c_buffer_size - c_header_size))
			return 0;
		return -4;
	}

	bool parse_body(const char* data, const size_t length, size_t& ndx)
	{
		size_t needlen = (bodylen_ + c_header_size) - pos_;
		if (needlen <= 0) return true;

		size_t buflen = length - ndx;
		if(buflen <= 0) return false;

		size_t cplen = buflen < needlen ? buflen : needlen;
		if (!inmsg_.append(data + ndx,  static_cast<int>(cplen)))
			return false;

		pos_ += cplen;
		ndx += cplen;

		if(pos_ < (bodylen_ + c_header_size))
			return false;

		return true;
	}

private:
	int state_;
	size_t	pos_;
	size_t bodylen_;
	char *buf_;
	InMessage inmsg_;
};

MessageParser* MessageParser::create(tcpconn* h)
{
	return new concrete_message_parser(h);
}
