#ifndef LOOPBUF_HEADER
#define LOOPBUF_HEADER

#include <stdlib.h>
#include <string.h>

class loopbuf  
{
public:
	char* buf_;
	char* wptr_; 
	char* rptr_; 
	char* hptr_;
	char* tptr_;
	unsigned long count_;

	loopbuf();
	loopbuf(unsigned long bufsize);
	virtual ~loopbuf();
	void init_with_zero();
	void init_with_size(unsigned long bufsize);
	void reset();
	unsigned long put(char* buf, unsigned long size);
	unsigned long get(char* buf, unsigned long size);
	unsigned long peek(char* buf, unsigned long size);
	unsigned long erase(unsigned long size);
	unsigned long count(); 
	unsigned long freecount();
	unsigned long datacount();	
};

#endif

