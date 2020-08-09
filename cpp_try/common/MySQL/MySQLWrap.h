/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MySQLWrap.h
* Summary: MySQL使用封装。
*
* Author: XuXiang
* Date: 2019-01-23 12:12
*******************************************************/

#ifndef __MySQLWrap_h__
#define __MySQLWrap_h__

#include <string>
#include <WinSock2.h>
#include <mysql.h>
#include <iostream>
#include <memory>
#include <assert.h>
#include "MySQLResult.h"
using namespace std;

namespace XX
{
	//游戏逻辑使用此类来访问MySQL(非线程安全)
	class MySQLWrap
	{
	public:
		MySQLWrap();
		virtual ~MySQLWrap();

		void Init(const char *host, const char *user, const char *passwd, const char *db, unsigned int port);

		void Release();

		//查询 sql:查询语句 ret:查询结果，需要调用者delete
		auto_ptr<MySQLResult> Query(const char *sql);

		//执行 sql:执行语句 ret:是否执行成功
		bool Execute(const char *sql);

		//心跳 保持MySQL连接活动状态
		void DoHeartBeat();

		//开始事务
		void StartTransaction();

		//结束事务
		inline void EndTransaction(bool ok);

		//提交事务
		void Commit();

		//回滚事务
		void RollBack();

	private:

		//开始查询
		bool StartMySQLQuery(const char *sql);

		//清除结果
		void ClearResult();

		MYSQL m_MySQL;
	};

	void MySQLWrap::EndTransaction(bool ok)
	{
		//成功就提交，不成功就回滚
		if (ok)
		{
			Commit();
		}
		else
		{
			RollBack();
		}
	}
}

#endif