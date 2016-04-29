#ifndef shared_const_buffer_header
#define shared_const_buffer_header

#include <memory>
#include <string.h>

class shared_const_buffer
{
public:
	shared_const_buffer(const char* p = NULL, size_t len = 0): left_(len), pos_(0)
	{
		memcpy(buffer_, p, len);
	}

	~shared_const_buffer()
	{
	}

	char* get()
	{
		return buffer_ + pos_;
	}

	void read(const size_t len)
	{
		left_ -= len;
		pos_ += len;
	}

	size_t size() const { return left_; }

private:
	enum { buffer_size = 1024 * 8 };
	size_t left_;
	size_t pos_;
	char buffer_[buffer_size];
};

#endif
