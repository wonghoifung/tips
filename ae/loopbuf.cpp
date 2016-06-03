#include "loopbuf.h"

loopbuf::loopbuf()
{
	init_with_zero();
}

loopbuf::loopbuf(unsigned long bufsize) 
{
	init_with_zero();
	init_with_size(bufsize); 
}

loopbuf::~loopbuf() 
{
	if (buf_) 
	{
		free(buf_); 
		buf_ = 0;
	}
}

void loopbuf::init_with_zero()
{
	buf_ = 0;	
	wptr_ = 0;
	rptr_ = 0;
	hptr_ = 0;
	tptr_ = 0;
	count_ = 0;
}

void loopbuf::init_with_size(unsigned long bufsize)
{		
	bufsize++;
	buf_ = (char*)malloc(bufsize);
	memset(buf_,0,bufsize);
	hptr_ = buf_;
	tptr_ = hptr_ + bufsize;
	wptr_ = rptr_ = hptr_;
	count_ = bufsize;
}

void loopbuf::reset()
{
	hptr_ = buf_;
	tptr_ = hptr_ + count_;
	wptr_ = rptr_ = hptr_;
}

unsigned long loopbuf::put(char* buf, unsigned long size)
{
	char* readptr	= rptr_;
	unsigned long part	= tptr_ - wptr_;

	if (wptr_ >= readptr)
	{
		if (part >= size)
		{				
			memcpy(wptr_, buf, size);
			wptr_ += size;
			return size;
		}
		else
		{
			memcpy(wptr_, buf, part);
			buf += part;
			size -= part;
			if (size > (unsigned long)(rptr_ - hptr_ - 1))
			{
				size = rptr_ - hptr_ - 1;
			}
			memcpy(hptr_, buf, size);
			wptr_ = hptr_ + size;
			return part + size;
		}
	}
	else 
	{
		if (size > (unsigned long)(readptr - wptr_ - 1))
		{
			size = readptr - wptr_ - 1;
		}
		memcpy(wptr_, buf, size);
		wptr_ += size;
		return size;
	}
}

unsigned long loopbuf::get(char* buf, unsigned long size) 
{
	char* writeptr	= wptr_;
	unsigned long part	= tptr_ - rptr_;

	if (writeptr >= rptr_)
	{
		if (size > (unsigned long)(writeptr - rptr_))
		{
			size = writeptr - rptr_;
		}
		memcpy(buf, rptr_, size);
		rptr_ += size;
		return size;
	}
	else
	{
		if (part >= size)
		{
			memcpy(buf, rptr_, size);
			rptr_ += size;
			return size;
		}
		else
		{
			memcpy(buf, rptr_, part);
			buf += part;
			size -= part;
			if (size > (unsigned long)(writeptr - hptr_))
			{
				size = writeptr - hptr_;
			}
			memcpy(buf, hptr_, size);
			rptr_ = hptr_ + size;
			return part + size;
		}
	}
}

unsigned long loopbuf::peek(char* buf, unsigned long size)
{
	char* writeptr	= wptr_;
	unsigned long part	= tptr_ - rptr_;

	if (writeptr >= rptr_)
	{
		if (size > (unsigned long)(writeptr - rptr_))
		{
			size = writeptr - rptr_;
		}
		memcpy(buf, rptr_, size);
		return size;
	}
	else
	{
		if (part >= size)
		{
			memcpy(buf, rptr_, size);
			return size;
		}
		else
		{
			memcpy(buf, rptr_, part);
			buf += part;
			size -= part;
			if (size > (unsigned long)(writeptr - hptr_))
			{
				size = writeptr - hptr_;
			}
			memcpy(buf, hptr_, size);
			return part + size;
		}
	}
}

unsigned long loopbuf::erase(unsigned long size)
{
	char* writeptr	= wptr_;
	unsigned long part	= tptr_ - rptr_;

	if (writeptr >= rptr_)
	{
		if (size > (unsigned long)(writeptr - rptr_))
		{
			size = writeptr - rptr_;
		}
		rptr_ += size;
		return size;
	}
	else
	{
		if (part >= size)
		{
			rptr_ += size;
			return size;
		}
		else
		{
			size -= part;
			if (size > (unsigned long)(writeptr - hptr_))
				size = writeptr - hptr_;
			rptr_ = hptr_ + size;
			return part + size;
		}
	}
}

unsigned long loopbuf::count() 
{ 
	return count_ - 1; 
}

unsigned long loopbuf::freecount() 
{
	char* writeptr	= wptr_;
	char* readptr	= rptr_;
	if (writeptr >= readptr)
	{
		return count_ - (writeptr - readptr) -1;
	}
	else
	{
		return (readptr - writeptr) -1;
	}
}

unsigned long loopbuf::datacount()
{
	char* writeptr	= wptr_;
	char* readptr	= rptr_;
	if (writeptr >= readptr)
	{
		return writeptr - readptr;
	}
	else
	{
		return count_ - (readptr - writeptr);
	}
}

