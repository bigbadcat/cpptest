/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MySQLResult.h
* Summary: MySQL查询结果。
*
* Author: XuXiang
* Date: 2019-01-23 15:12
*******************************************************/

#ifndef __MySQLResult_h__
#define __MySQLResult_h__

#include <string>
#include <vector>
#include <map>
#include "../Macro.h"
using namespace std;

typedef struct st_mysql_res MYSQL_RES;
typedef char **MYSQL_ROW;

namespace XX
{
	//游戏逻辑使用此类来读取查询数据
	class MySQLResult
	{
		typedef map<string, int> NameIndexMap;

	public:
		MySQLResult();
		virtual ~MySQLResult();

		//初始化
		void Init(MYSQL_RES *res);

		//获取一个记录，用例while(res->GetRecord()){DoRead();}
		//ret:返回true后用GetXXX函数读取数据，返回false表示没有数据了
		bool GetRecord();

		//获取名称对应的索引
		int GetIndex(const char *name);

		//获取字段名列表
		const vector<string>& GetFieldNames()const { return m_FieldNames; }

		//获取整数
		int GetInt(int index);
		inline int GetInt(const char *name) { return GetInt(GetIndex(name)); }

		//获取长整数
		Int64 GetInt64(int index);
		inline Int64 GetInt64(const char *name) { return GetInt64(GetIndex(name)); }

		//获取浮点数
		float GetFloat(int index);
		inline float GetFloat(const char *name) { return GetFloat(GetIndex(name)); }

		//获取字符串 ret:若要保存需拷贝一份
		const char* GetString(int index);
		inline const char* GetString(const char *name) { return GetString(GetIndex(name)); }

		//清除结果
		void Clear();

	private:

		//字段名称列表
		vector<string> m_FieldNames;

		//名称到索引的映射
		NameIndexMap m_NameToIndex;

		//查询结果
		MYSQL_RES *m_Result;

		//当前记录
		MYSQL_ROW m_CurRecord;
	};
}

#endif