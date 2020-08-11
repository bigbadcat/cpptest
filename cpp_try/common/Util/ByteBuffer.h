/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: ByteBuffer.h
* Summary: 字节缓冲区。
*
* Author: XuXiang
* Date: 2018-11-24 12:15
*******************************************************/

#ifndef __ByteBuffer_h__
#define __ByteBuffer_h__

#include "../Macro.h"
#include <string>
using namespace std;

namespace XX
{
	class ByteBuffer
	{
	public:
		//构造析构函数
		ByteBuffer(int size);
		~ByteBuffer();

		//获取缓冲区大小
		inline int GetSize()const { return m_Size; }

		//获取缓冲区数据长度
		inline int GetLength()const { return m_Length; }

		//获取数据
		inline Byte* GetData()const { return m_pData; }

		//添加数据
		bool AddData(Byte* data, int len);

		//获取数据
		void GetData(Byte* dst, int index, int len);

		//移除数据
		void RemoveData(int len);

		//清除数据
		void Clear() { m_Length = 0; }

	private:
		//缓冲区数据
		Byte *m_pData;

		//缓冲区长度
		int m_Size;

		//当前数据长度
		int m_Length;

	};
}

#endif