/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetListener.h
* Summary: 网络通信模块服务端连接监听线程。
*
* Author: XuXiang
* Date: 2018-07-30 22:58
*******************************************************/

#ifndef __NetListener_h__
#define __NetListener_h__

#include "NetDefine.h"

namespace Net
{
	class NetServer;

	//服务端的网络连接监听者		
	class NetListener
	{
	public:
		//构造析构函数
		NetListener();
		virtual ~NetListener();

		//启动监听。
		//port:端口号。
		void Start(int port);

		//停止监听
		void Stop();

		//设置服务端
		inline void SetServer(NetServer *p) { m_pServer = p; }

		//获取服务端
		inline NetServer* GetServer()const { return m_pServer; }

		//获取套接字
		inline socket_t GetSocket()const { return m_Socket; }

		//Socket可以读取数据了
		int OnSocketRead();

		//Socket关闭
		void OnSocketClose();

	private:

		//创建监听Socket
		socket_t CreateListenSocket();

		//Socket句柄
		socket_t m_Socket;

		//端口号。
		int m_Port;

		//网络服务端
		NetServer *m_pServer;
	};
}

#endif