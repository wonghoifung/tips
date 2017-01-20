#include "stdafx.h"
#include "random.h"

// TODO where do this algorithm come from?

CRandom::CRandom()
{
}

CRandom::~CRandom()
{
}

BOOL CRandom::Init(UINT seed)
{
	mSeed = seed;
	return TRUE;
}

INT CRandom::Rand()
{
	return( ((mSeed = mSeed * 214013L + 2531011L) >> 16) & 0x7FFF );
}

INT CRandom::SimpleRandom()
{
	return( ((mSeed = mSeed + 2531011L) >> 3) & 0x7FFF );
}

INT CRandom::ComplexRandom()
{
	UINT next = mSeed;
	INT  result;

	next *= 1103515245;
	next += 12345;
	result = (UINT) (next / 65536) % 2048;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT) (next / 65536) % 1024;

	next *= 1103515245;
	next += 12345;
	result <<= 10;
	result ^= (UINT) (next / 65536) % 1024;

	mSeed = next;
	return result;
}
