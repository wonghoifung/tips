#pragma once

class CRandom
{
public:
	CRandom();
	virtual ~CRandom();
	BOOL Init(UINT seed);
	INT Rand();
	INT SimpleRandom();
	INT ComplexRandom();

private:
	UINT	mSeed;
};
