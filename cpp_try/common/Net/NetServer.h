/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetServer.h
* Summary: 网络通信模块服务端。
*
* Author: XuXiang
* Date: 2018-07-30 22:49
*******************************************************/

#ifndef __NetServer_h__
#define __NetServer_h__

#include "NetDefine.h"
#include <thread>
using namespace std;

namespace XX
{
	class MySQLWrap;
}

namespace Net
{
	class NetListener;
	class NetConnection;
	class NetConnectionManager;

	//服务端网络管理。		
	class NetServer
	{
	public:
		//构造析构函数
		NetServer();
		~NetServer();

		void Init();

		//启动网络模块。port:监听的端口号。
		void Start(int port);

		//停止服务
		void Stop();

		//有连接接收到数据包
		void OnRecvData(NetConnection* con);

	private:

		//线程过程
		void ThreadProcess();

		int m_Port;

		//MySQL
		XX::MySQLWrap *m_MySQL;

		//连接管理
		NetConnectionManager *m_pConnectionManager;

		//网络处理线程
		thread m_Thread;

		bool m_Run;
	};
}

#endif