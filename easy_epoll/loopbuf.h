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
	void InitMember();
	void Init(unsigned long bufsize);
	void Reset();
	unsigned long Put(char* buf, unsigned long size);
	unsigned long Get(char* buf, unsigned long size);
	unsigned long Peek(char* buf, unsigned long size);
	unsigned long Erase(unsigned long size);
	unsigned long Count(); 
	unsigned long FreeCount();
	unsigned long DataCount();	
};

#endif

