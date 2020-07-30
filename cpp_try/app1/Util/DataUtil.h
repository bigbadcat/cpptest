/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: DataUtil.h
* Summary: 数据通用操作函数。
*
* Author: XuXiang
* Date: 2018-08-26 22:23
*******************************************************/

#ifndef __DataUtil_h__
#define __DataUtil_h__

#include "../Macro.h"
#include <string>
using namespace std;

class DataUtil
{
public:

	//写入一个64位无符号整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static int WriteUInt64(Byte *src, int index, UInt64 value);

	//写入一个64位整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static inline int WriteInt64(Byte *src, int index, Int64 value) { return WriteUInt64(src, index, (UInt64)value); }

	//读取一个64位无符号整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static UInt64 ReadUInt64(Byte *src, int index, int *next);

	//读取一个64位整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static inline Int64 ReadInt64(Byte *src, int index, int *next) { return (Int64)ReadUInt64(src, index, next); }

	//写入一个32位无符号整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static int WriteUInt32(Byte *src, int index, UInt32 value);

	//写入一个32位整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static inline int WriteInt32(Byte *src, int index, Int32 value) { return WriteUInt32(src, index, (UInt32)value); }

	//读取一个32位无符号整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static UInt32 ReadUInt32(Byte *src, int index, int *next);

	//读取一个32位整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static inline Int32 ReadInt32(Byte *src, int index, int *next) { return (Int32)ReadUInt32(src, index, next); }

	//写入一个16位无符号整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static int WriteUInt16(Byte *src, int index, UInt16 value);

	//写入一个16位整数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static inline int WriteInt16(Byte *src, int index, Int16 value) { return WriteUInt16(src, index, (UInt16)value); }

	//读取一个16位无符号整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static UInt16 ReadUInt16(Byte *src, int index, int *next);

	//读取一个16位整数。 
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static inline Int16 ReadInt16(Byte *src, int index, int *next) { return (Int16)ReadUInt16(src, index, next); }

	//写入一个浮点数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static int WriteFloat(Byte *src, int index, float value);

	//读取一个浮点数。
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static float ReadFloat(Byte *src, int index, int *next);

	//写入一个双精度浮点数。
	//src:要写入的字节数组。
	//index:要写入的位置。
	//value:要写入的值。
	//ret:下一个写入位置。
	static int WriteDouble(Byte *src, int index, double value);

	//读取一个双精度浮点数。
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//ret:读取的值。
	static double ReadDouble(Byte *src, int index, int *next);

	//写入一个字符串。
	//要写入的字节数组。
	//要写入的起始位置。
	//要写入的值。
	//写入长度。
	//新的起始位置。
	static int WriteString(Byte *src, int index, const string& value, int n = 0);

	//读取一个双精度浮点数。
	//src:要读取的字节数组。
	//index:要读取的位置。
	//next:[out]下一个读取位置。
	//读取长度。
	//ret:读取的值。
	static string ReadString(Byte *src, int index, int *next, int n = 0);
};

#endif