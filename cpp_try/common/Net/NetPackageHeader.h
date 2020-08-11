/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetPackageHeader.h
* Summary: 数据包头。
*
* Author: XuXiang
* Date: 2018-07-30 23:08
*******************************************************/

#ifndef __NetPackageHeader_h__
#define __NetPackageHeader_h__

#include"../Macro.h"

namespace Net
{
	//数据包头部。
	struct NetPackageHeader
	{
		//数据包头签名始终为固定值0x58。(X)
		static const Byte HEADER_SIGN = 0x58;

		//包头大小。
		static const int HEADER_SIZE = 1 + 1 + 1 + 1 + 4 + 2 + 2 + 4 + 8 + 4;

		//包头签名。
		Byte Sign;

		//随机种子，用来生成CheckSum。
		Byte RandSeed;

		//检验和, 生成见后文函数。
		Byte CheckSum;

		//节点
		Byte Node;

		//压缩参数，0表示未压缩。
		Int32 Compress;

		//包的序列号，从0 ~9999 ，必须循环递增，用于底层处理断线重连。
		Int16 Index;

		//接收到的包的序列号。
		Int16 RecvIndex;

		//协议消息码。
		Int32 Command;

		//玩家角色
		Int64 GUID;

		//包体大小（字节数）。
		Int32 BodySize;

		//构造函数
		NetPackageHeader();

		//重置数据。
		void Reset();

		//设置签名
		void SetSign();

		//解包头部。
		//src:要读取的字节数组。
		//index:要读取的位置。
		//next:[out]下一个读取位置。
		//ret:下一个读取位置。
		int Unpack(Byte *src, int index);

		//打包头部。
		//src:要写入的字节数组。
		//index:要写入的位置。
		//ret:下一个写入位置。
		int Pack(Byte *src, int index);
	};
}

#endif