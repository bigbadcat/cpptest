/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnection.h
* Summary: 网络通信模块服务端连接。
*
* Author: XuXiang
* Date: 2018-07-30 23:06
*******************************************************/

#ifndef __NetConnection_h__
#define __NetConnection_h__

#include <map>
#include "../Macro.h"
#include "NetDefine.h"
#include "NetPackageHeader.h"
#include "../Util/ByteBuffer.h"
using namespace XX;

namespace Net
{
	class NetConnectionManager;

	//网络连接对象
	class NetConnection
	{
	public:
		NetConnection();
		virtual ~NetConnection();

		//获取唯一编号
		inline Int64 GetUniqueID()const { return  (Int64)GetSocket(); }

		//获取Sokcet ret:Socket
		inline socket_t GetSocket()const { return m_Socket; }

		//判断是否需要读数据
		inline bool IsNeedRead() { return NET_BUFFER_SIZE - m_RecvBuffer.GetLength() >= NET_PACKAGE_MAX_SIZE; }

		//判断是否需要写数据
		inline bool IsNeedWrite() { return m_SendBuffer.GetLength() > 0; }

		//设置Socket
		virtual void SetSocket(socket_t s);

		//获取远端类型
		inline int GetRomoteType()const { return m_RemoteType; }

		//设置远端类型
		inline void SetRomoteType(int type) { m_RemoteType = type; }

		//获取角色id
		inline Int64 GetRoleID()const { return m_RoleID; }

		//设置角色id
		inline void SetRoleID(Int64 roleid) { m_RoleID = roleid; }

		//获取IP地址
		inline const std::string& GetIPAddress()const { return m_IPAddress; }

		//获取端口号
		inline const int GetPort()const { return m_Port; }

		//发送数据
		bool Send(Int64 guid, Int32 cmd, Byte *buffer, int len);

		//添加发送的数据
		bool AddSendData(Byte *buffer, int len);

		//添加接收的数据
		bool AddRecvData(Byte *buffer, int len);

		//检测是否接受到了完整数据包
		//ret:0 没有完整数据包 1:有完整数据包 -1:数据错误
		int CheckRecvPackage();

		//拿去接收到的数据包
		void TakeRecvPackage(NetPackageHeader& header, Byte *buffer, int size);

		//进行发送操作
		int DoSend();

		//设置连接管理
		inline void SetManager(NetConnectionManager *p) { m_BelongManager = p; }

		//获取连接管理
		inline NetConnectionManager* GetManager()const { return m_BelongManager; }

	private:

		//所属管理者
		NetConnectionManager *m_BelongManager;

		//Socket句柄，连接唯一标识
		socket_t m_Socket;

		//远端类型
		int m_RemoteType;

		//角色标识(仅客户端连接有用)
		Int64 m_RoleID;

		//IP地址。
		std::string m_IPAddress;

		//端口号。
		int m_Port;

		//收发缓冲区
		ByteBuffer m_SendBuffer;

		//接收缓冲区
		ByteBuffer m_RecvBuffer;

		//接收的数据头
		NetPackageHeader m_RecvHeader;
	};
}

#endif