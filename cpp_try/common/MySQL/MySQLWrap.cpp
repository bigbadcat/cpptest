/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: MySQLWrap.cpp
* Summary: MySQL使用封装。
*
* Author: XuXiang
* Date: 2019-01-23 12:12
*******************************************************/

#include "MySQLWrap.h"

namespace XX
{
	MySQLWrap::MySQLWrap()
	{		
	}

	MySQLWrap::~MySQLWrap()
	{		
	}

	void MySQLWrap::Init(const char *host, const char *user, const char *passwd, const char *db, unsigned int port)
	{
		mysql_init(&m_MySQL);
		
		MYSQL *ret = mysql_real_connect(&m_MySQL, host, user, passwd, db, port, NULL, 0);
		if (ret == NULL)
		{
			const char *error_str = mysql_error(&m_MySQL);
			cout << error_str << endl;
			assert(false);
		}
	}

	void MySQLWrap::Release()
	{
		mysql_close(&m_MySQL);
	}

	auto_ptr<MySQLResult> MySQLWrap::Query(const char *sql)
	{
		if (!StartMySQLQuery(sql))
		{
			assert(false);
		}

		MYSQL_RES *res = mysql_store_result(&m_MySQL);
		assert(res != NULL);
		MySQLResult *result = new MySQLResult();
		result->Init(res);
		return auto_ptr<MySQLResult>(result);
	}

	bool MySQLWrap::Execute(const char *sql)
	{
		return StartMySQLQuery(sql);
	}

	void MySQLWrap::DoHeartBeat()
	{
		//printf("MySQLWrap::DoHeartBeat\n");
		Execute("call sp_update_heart_beat");
	}

	void MySQLWrap::StartTransaction()
	{
		ClearResult();
		mysql_query(&m_MySQL, "START TRANSACTION");
	}

	void MySQLWrap::Commit()
	{
		mysql_query(&m_MySQL, "COMMIT");
	}

	void MySQLWrap::RollBack()
	{
		mysql_query(&m_MySQL, "ROLLBACK");
	}

	bool MySQLWrap::StartMySQLQuery(const char *sql)
	{
		//把之前剩余的结果清掉		
		ClearResult();
		if (mysql_query(&m_MySQL, sql) != 0)
		{
			const char *error_str = mysql_error(&m_MySQL);
			cout << error_str << endl;
			return false;
		}
		return true;
	}

	void MySQLWrap::ClearResult()
	{
		while (!mysql_next_result(&m_MySQL))
		{
			MYSQL_RES *result = mysql_store_result(&m_MySQL);
			mysql_free_result(result);
		}
	}
}