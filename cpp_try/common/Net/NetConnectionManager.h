/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnectionManager.h
* Summary: 网络通信模块服务端连接管理器。
*
* Author: XuXiang
* Date: 2018-07-30 23:07
*******************************************************/

#ifndef __NetConnectionManager_h__
#define __NetConnectionManager_h__

#include <vector>
#include <map>
#include "NetDefine.h"

namespace Net
{
	class NetServer;
	class NetListener;
	class NetConnection;

	//用于唤醒接收阻塞的线程
	class NetAwakeBridge
	{
	public:
		NetAwakeBridge();
		~NetAwakeBridge();

		//初始化
		void Init(int port);

		//通知唤醒
		void Notify();

	private:

		bool CheckSend();

		socket_t m_Send;
	};

	//网络连接管理，通过管理多个连接线程实现
	class NetConnectionManager
	{
		typedef std::map<socket_t, NetConnection*> NetConnectionMap;

	public:
		//构造析构函数
		NetConnectionManager();
		virtual ~NetConnectionManager();

		//初始化
		void Init(int port);

		//释放
		void Release();

		//选择Socket处理
		void SelectSocket(int msec);

		//设置服务端
		inline void SetServer(NetServer *p) { m_pServer = p; }

		//获取服务端
		inline NetServer* GetServer()const { return m_pServer; }

		//添加连接
		void AddConnection(socket_t s);

		//移除并关闭连接
		void RemoveConnection(NetConnection* con);

		//获取连接
		NetConnection* GetConnection(UInt64 uid)const { return GetConnectionFromSocket((socket_t)uid); }

		//有连接接收到数据包了
		void OnRecvPackage(NetConnection *con);

		//通知唤醒
		void NotifyAwake() { m_AwakeBridge.Notify(); }

		//更新发送状态
		void UpdateConnectionSend(NetConnection* con);

	private:

#ifdef  WIN
		//选择Socket处理
		void SelectSocketWin(int msec);
#else
		//选择Socket处理
		void SelectSocketLinux(int msec);
#endif

		//获取连接
		NetConnection* GetConnectionFromSocket(socket_t s)const;

		//Socket可以读取数据了
		int OnSocketRead(socket_t s);

		//Socket可以写入数据了
		int OnSocketWrite(socket_t s);

		//Socket关闭
		void OnSocketClose(socket_t s);

		//网络服务端
		NetServer *m_pServer;

		//监听者
		NetListener *m_pListener;

		//唤醒对象
		NetAwakeBridge m_AwakeBridge;

		//连接集合
		NetConnectionMap m_Connections;

#ifndef WIN
		int m_EpollFD;
		epoll_event *m_Events;
#endif

	};
}

#endif