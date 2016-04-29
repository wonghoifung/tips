//
//  Created by huanghaifeng on 15-9-1.
//  Copyright (c) 2015 wonghoifung. All rights reserved.
//
#ifndef ENDIAN_OP_HEADER
#define ENDIAN_OP_HEADER

#include <stdint.h>
#include <boost/assert.hpp>

#if defined(_WIN32) || defined(__APPLE__) 

	#define IS_LITTLE_ENDIAN 
	
#else

    #include <endian.h> 
	
	#if (__BYTE_ORDER == __BIG_ENDIAN)
		#define IS_BIG_ENDIAN
	#elif (__BYTE_ORDER == __LITTLE_ENDIAN)
		#define IS_LITTLE_ENDIAN
	#else
		#error endianness unknown
	#endif

#endif /// defined(_WIN32) || defined(__APPLE__)


#ifdef IS_BIG_ENDIAN

	template <class T>
	inline T endian_op(const T value) 
	{
		return value;
	}

#else /* IS_LITTLE_ENDIAN */

	template <class T>
	inline T endian_op(const T value)
	{
		T result;
		const size_t len = sizeof(T);
		switch (len)
		{
		case 1:
			return value;
		case 2:
			((unsigned char*)&result)[0] = ((const unsigned char*)&value)[1];
			((unsigned char*)&result)[1] = ((const unsigned char*)&value)[0];
			return result;
		case 4:
			((unsigned char*)&result)[0] = ((const unsigned char*)&value)[3];
			((unsigned char*)&result)[1] = ((const unsigned char*)&value)[2];
			((unsigned char*)&result)[2] = ((const unsigned char*)&value)[1];
			((unsigned char*)&result)[3] = ((const unsigned char*)&value)[0];
			return result;
		case 8:
			((unsigned char*)&result)[0] = ((const unsigned char*)&value)[7];
			((unsigned char*)&result)[1] = ((const unsigned char*)&value)[6];
			((unsigned char*)&result)[2] = ((const unsigned char*)&value)[5];
			((unsigned char*)&result)[3] = ((const unsigned char*)&value)[4];
			((unsigned char*)&result)[4] = ((const unsigned char*)&value)[3];
			((unsigned char*)&result)[5] = ((const unsigned char*)&value)[2];
			((unsigned char*)&result)[6] = ((const unsigned char*)&value)[1];
			((unsigned char*)&result)[7] = ((const unsigned char*)&value)[0];
			return result;
		default:
			break;
		}
		BOOST_ASSERT(false);
		return 0;
	}

#endif /// IS_BIG_ENDIAN


#endif /// ENDIAN_OP_HEADER
