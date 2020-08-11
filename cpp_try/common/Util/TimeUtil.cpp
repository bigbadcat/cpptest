/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: TimeUtil.cpp
* Summary: 时间相关通用操作函数。
*
* Author: XuXiang
* Date: 2018-10-29 11:10
*******************************************************/

#include "TimeUtil.h"

namespace XX
{
	Int64 TimeUtil::StartSecond = 0;

	Int64 TimeUtil::StartTickCount = 0;

	Int64 TimeUtil::GetCurrentMillisecond()
	{
		//当前毫秒=启动时刻秒+(当前tick-启动时刻tick)
		if (StartSecond == 0)
		{
			StartSecond = GetCurrentSecond() * 1000;
			StartTickCount = GetTickCount();
		}

		//clock或取的tick只能表示24.8天，到时候会重新开始
		Int64 nowtick = GetTickCount();
		if (nowtick < StartTickCount)
		{
			StartSecond = GetCurrentSecond() * 1000;
			StartTickCount = nowtick;
		}
		Int64 gap = nowtick - StartTickCount;
		Int64 ret = StartSecond + gap;
		return ret;
	}
}