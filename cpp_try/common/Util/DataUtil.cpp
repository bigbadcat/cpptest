/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: DataUtil.cpp
* Summary: 数据通用操作函数。
*
* Author: XuXiang
* Date: 2018-08-26 22:26
*******************************************************/

#include "DataUtil.h"
#include <algorithm>
#include <assert.h>
#include <cstring>

namespace XX
{
	int DataUtil::WriteUInt64(Byte *src, int index, UInt64 value)
	{
		src[index + 7] = (Byte)((value >> 56) & 0xFF);
		src[index + 6] = (Byte)((value >> 48) & 0xFF);
		src[index + 5] = (Byte)((value >> 40) & 0xFF);
		src[index + 4] = (Byte)((value >> 32) & 0xFF);
		src[index + 3] = (Byte)((value >> 24) & 0xFF);
		src[index + 2] = (Byte)((value >> 16) & 0xFF);
		src[index + 1] = (Byte)((value >> 8) & 0xFF);
		src[index + 0] = (Byte)(value & 0xFF);
		return index + 8;
	}

	UInt64 DataUtil::ReadUInt64(Byte *src, int index, int *next)
	{		
		UInt64 b7 = (UInt64)(src[index + 7]) << 56;
		UInt64 b6 = (UInt64)(src[index + 6]) << 48;
		UInt64 b5 = (UInt64)(src[index + 5]) << 40;
		UInt64 b4 = (UInt64)(src[index + 4]) << 32;
		UInt64 b3 = (UInt64)(src[index + 3]) << 24;
		UInt64 b2 = (UInt64)(src[index + 2]) << 16;
		UInt64 b1 = (UInt64)(src[index + 1]) << 8;
		UInt64 b0 = (UInt64)(src[index + 0]);
		UInt64 value = b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0;
		*next += 8;
		return value;
	}

	int DataUtil::WriteUInt32(Byte *src, int index, UInt32 value)
	{
		src[index + 3] = (Byte)((value >> 24) & 0xFF);
		src[index + 2] = (Byte)((value >> 16) & 0xFF);
		src[index + 1] = (Byte)((value >> 8) & 0xFF);
		src[index + 0] = (Byte)(value & 0xFF);
		return index + 4;
	}

	UInt32 DataUtil::ReadUInt32(Byte *src, int index, int *next)
	{
		UInt32 b3 = (UInt32)(src[index + 3]) << 24;
		UInt32 b2 = (UInt32)(src[index + 2]) << 16;
		UInt32 b1 = (UInt32)(src[index + 1]) << 8;
		UInt32 b0 = (UInt32)(src[index + 0]);
		UInt32 value = b3 | b2 | b1 | b0;
		*next += 4;
		return value;
	}

	int DataUtil::WriteUInt16(Byte *src, int index, UInt16 value)
	{
		src[index + 1] = (Byte)((value >> 8) & 0xFF);
		src[index + 0] = (Byte)(value & 0xFF);
		return index + 2;
	}

	UInt16 DataUtil::ReadUInt16(Byte *src, int index, int *next)
	{
		UInt16 b1 = (UInt16)(src[index + 1]) << 8;
		UInt16 b0 = (UInt16)(src[index + 0]);
		UInt16 value = b1 | b0;
		*next += 2;
		return value;
	}

	int DataUtil::WriteFloat(Byte *src, int index, float value)
	{
		Byte *p = (Byte*)&value;
		::memcpy(src + index, p, sizeof(float));
		return index + sizeof(float);
	}

	float DataUtil::ReadFloat(Byte *src, int index, int *next)
	{
		float *p = (float*)(src + index);
		float ret = *p;
		*next += sizeof(float);
		return ret;
	}

	int DataUtil::WriteDouble(Byte *src, int index, double value)
	{
		Byte *p = (Byte*)&value;
		::memcpy(src, p, sizeof(double));
		return index + sizeof(double);
	}

	double DataUtil::ReadDouble(Byte *src, int index, int *next)
	{
		double *p = (double*)(src + index);
		double ret = *p;
		*next += sizeof(double);
		return ret;
	}

	int DataUtil::WriteString(Byte *src, int index, const string& value, int n)
	{
		size_t len = value.size() + 1;
		assert(len <= 0x7FFFFFFF);
		assert(n >= 0);
		if (n == 0)
		{
			//写入长度 再写入对应字节数组			
			index = WriteInt32(src, index, (int)len);
			::strcpy((char*)src + index, value.c_str());
			index += (int)len;
		}
		else
		{
			int num = ::min((int)len, n);
			::memcpy(src + index, value.c_str(), num);
			//::strcpy_s((char*)src + index, num, value.c_str());
			if (num < n)
			{
				//后面的补0
				::memset(src + index + num, 0, n - num);
			}
			index += n;
		}
		return index;
	}
	
	string DataUtil::ReadString(Byte *src, int index, int *next, int n)
	{
		assert(n >= 0);
		int len = n == 0 ? ReadInt32(src, index, &index) : n;
		size_t strlen = ::strnlen((char*)src + index, len);	//要读取真实的字符串长度，否则会有'\0'字符混入string对象中
		string ret((char*)src + index, strlen);		//STL的string允许'\0'字符作为string对象元素
		*next = index + len;
		return ret;
	}
}
