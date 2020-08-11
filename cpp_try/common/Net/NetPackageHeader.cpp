/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetPackageHeader.cpp
* Summary: 数据包头。
*
* Author: XuXiang
* Date: 2018-07-30 23:08
*******************************************************/

#include "NetPackageHeader.h"
#include "../Util/DataUtil.h"
using namespace XX;

namespace Net
{
	NetPackageHeader::NetPackageHeader(): Sign(0), RandSeed(0), CheckSum(0), Node(0), Compress(0), Index(0), RecvIndex(0),
		Command(0), GUID(0), BodySize(0)
	{

	}

	void NetPackageHeader::Reset()
	{
		Sign = 0;
		RandSeed = 0;
		CheckSum = 0;
		Compress = 0;
		Index = 0;
		RecvIndex = 0;
		Command = 0;
		Node = 0;
		GUID = 0;
		BodySize = 0;
	}

	void NetPackageHeader::SetSign()
	{
		Sign = HEADER_SIGN;
	}

	int NetPackageHeader::Unpack(Byte *src, int index)
	{
		int i = index;
		Sign = src[i++];
		RandSeed = src[i++];
		CheckSum = src[i++];
		Node = src[i++];
		Compress = DataUtil::ReadInt32(src, i, &i);
		Index = DataUtil::ReadInt16(src, i, &i);
		RecvIndex = DataUtil::ReadInt16(src, i, &i);
		Command = DataUtil::ReadInt32(src, i, &i);
		GUID = DataUtil::ReadInt64(src, i, &i);
		BodySize = DataUtil::ReadInt32(src, i, &i);
		return i;
	}

	int NetPackageHeader::Pack(Byte *src, int index)
	{
		int i = index;
		src[i++] = Sign;
		src[i++] = RandSeed;
		src[i++] = CheckSum;
		src[i++] = Node;
		i = DataUtil::WriteInt32(src, i, Compress);
		i = DataUtil::WriteInt16(src, i, Index);
		i = DataUtil::WriteInt16(src, i, RecvIndex);
		i = DataUtil::WriteInt32(src, i, Command);
		i = DataUtil::WriteInt64(src, i, GUID);
		i = DataUtil::WriteInt32(src, i, BodySize);
		return i;
	}
}