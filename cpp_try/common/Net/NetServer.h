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

		//添加连接
		void AddConnection(socket_t s);

	private:

		//线程过程
		void ThreadProcess();

		int m_Port;

		//MySQL
		XX::MySQLWrap *m_MySQL;

		//监听者
		NetListener *m_pListener;

		socket_t m_Socket;

		//网络处理线程
		thread m_Thread;

		bool m_Run;
	};
}

#endif