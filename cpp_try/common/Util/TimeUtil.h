/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: TimeUtil.h
* Summary: 时间相关通用操作函数。
*
* Author: XuXiang
* Date: 2018-10-29 11:10
*******************************************************/

#ifndef __TimeUtil_h__
#define __TimeUtil_h__

#include "../Macro.h"
#include <string>
#include <time.h>
using namespace std;

namespace XX
{
	class TimeUtil
	{
	public:

		//获取程序启动后经过毫秒数
		inline static Int64 GetTickCount() {return clock() / (CLOCKS_PER_SEC / 1000);}

		//获取当前的秒时间。
		static Int64 GetCurrentSecond() { return (Int64)::time(NULL); }

		//获取当前的毫秒时间。
		static Int64 GetCurrentMillisecond();

	private:

		//获取毫秒时间参照的起始时间秒
		static Int64 StartSecond;

		//获取毫秒时间参照的毫秒计数
		static Int64 StartTickCount;
	};
}

#endif