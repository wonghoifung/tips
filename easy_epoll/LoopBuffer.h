#ifndef LOOPBUFFER_HEADER
#define LOOPBUFFER_HEADER

#include <stdlib.h>
#include <string.h>

class LoopBuffer  
{
public:
	char* buf_;
	char* wptr_; 
	char* rptr_; 
	char* hptr_;
	char* tptr_;
	unsigned long count_;

	LoopBuffer();
	LoopBuffer(unsigned long bufsize);
	virtual ~LoopBuffer();
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

