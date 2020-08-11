/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: StringUtil.cpp
* Summary: 字符串通用操作。
*
* Author: XuXiang
* Date: 2018-12-03 20:13
*******************************************************/

#include "StringUtil.h"

namespace XX
{
	std::string& StringUtil::Trim(std::string &s)
	{
		if (s.empty())
		{
			return s;
		}

		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		return s;
	}

	vector<string>& StringUtil::Split(const std::string &s, const std::string& sp, vector<string>& v)
	{
		if (s.empty())
		{
			return v;
		}

		size_t last = 0;
		size_t index = s.find_first_of(sp, last);
		while (index != std::string::npos)
		{
			v.push_back(s.substr(last, index - last));
			last = index + 1;
			index = s.find_first_of(sp, last);
		}
		if (index - last > 0)
		{
			v.push_back(s.substr(last, index - last));
		}

		return v;
	}

	vector<int>& StringUtil::SplitToInt(const std::string &s, const std::string& sp, vector<int>& v)
	{
		vector<string> vs;		
		Split(s, sp, vs);

		for (vector<string>::iterator itr = vs.begin(); itr != vs.end(); ++itr)
		{
			int i = ::atoi(itr->c_str());
			v.push_back(i);
		}
		return v;
	}
		
	int StringUtil::GetStringWidth(const string& str)
	{
		int num = 0;
		int index = 0;
		while (index < str.length())
		{
			bool cn = (str[index] & 0x80) != 0;
			num += cn ? 2 : 1;			//汉字宽度为2
			index += cn ? 3 : 1;		//汉字UTF-8占三个字节(遇到占两个字节的字符编码会不正确)
		}
		return num;
	}
}