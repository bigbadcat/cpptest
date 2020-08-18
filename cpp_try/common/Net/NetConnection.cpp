/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetConnection.cpp
* Summary: 网络通信模块服务端连接。
*
* Author: XuXiang
* Date: 2018-07-30 23:06
*******************************************************/

#include "NetConnection.h"
#include "../Util/DataUtil.h"
#include <iostream>
#include <string>
using namespace std;

namespace Net
{
	NetConnection::NetConnection(): m_RoleID(0), m_Socket(SOCKET_ERROR), m_Port(0),
		m_SendBuffer(NET_BUFFER_SIZE), m_RecvBuffer(NET_BUFFER_SIZE)
	{
	}

	NetConnection::~NetConnection()
	{
		SAFE_CLOSE_SOCKET(m_Socket);
	}

	void NetConnection::SetSocket(socket_t s)
	{
		m_Socket = s;
		m_IPAddress = "";
		m_Port = 0;
		if (s == SOCKET_ERROR)
		{
			return;
		}

		sockaddr_in addr;
		socklen_t addr_len = sizeof(addr);
		::memset(&addr, 0, addr_len);
		int ret = getpeername(m_Socket, (struct sockaddr*)&addr, &addr_len);
		if (ret == 0)
		{
			if (addr.sin_family == AF_INET)
			{
				char ip[32];
				strncpy(ip, inet_ntoa(addr.sin_addr), 24);
				m_IPAddress = ip;
				m_Port = htons(addr.sin_port);
				::printf("accept connetion ip:%s port:%d\n", m_IPAddress.c_str(), m_Port);				
			}
			else
			{
				cout << "socket addr.sin_family:" << addr.sin_family << endl;
			}
		}
		else
		{
			cout << "getpeername err:" << GET_LAST_ERROR() << endl;
		}
	}

	bool NetConnection::Send(Int64 guid, Int32 cmd, Byte *buffer, int len)
	{
		NetPackageHeader sendhead;
		sendhead.SetSign();
		sendhead.Command = cmd;
		sendhead.GUID = guid;
		sendhead.BodySize = len;

		static Byte sendbuff[1024];
		int sendsize = 0;
		sendsize = sendhead.Pack(sendbuff, sendsize);
		::memcpy(sendbuff + sendsize, buffer, len);
		sendsize += len;

		return AddSendData(sendbuff, sendsize);
	}

	bool NetConnection::AddSendData(Byte *buffer, int len)
	{
		return m_SendBuffer.AddData(buffer, len);
	}

	bool NetConnection::AddRecvData(Byte *buffer, int len)
	{
		return m_RecvBuffer.AddData(buffer, len);
	}

	int NetConnection::CheckRecvPackage()
	{
		if (m_RecvHeader.Sign == 0)
		{
			if (m_RecvBuffer.GetLength() >= NetPackageHeader::HEADER_SIZE)
			{
				m_RecvHeader.Unpack(m_RecvBuffer.GetData(), 0);
				if (m_RecvHeader.Sign != NetPackageHeader::HEADER_SIGN || m_RecvHeader.BodySize > NET_PACKAGE_MAX_SIZE)
				{
					return -1;
				}
			}
		}

		bool have = m_RecvHeader.Sign != 0 && m_RecvBuffer.GetLength() >= (m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE);
		return have ? 1 : 0;
	}

	void NetConnection::TakeRecvPackage(NetPackageHeader& header, Byte *buffer, int size)
	{
		//获取数据包
		int datasize = m_RecvHeader.BodySize + NetPackageHeader::HEADER_SIZE;
		header = m_RecvHeader;
		m_RecvBuffer.GetData(buffer, NetPackageHeader::HEADER_SIZE, m_RecvHeader.BodySize);

		//清除数据包
		m_RecvBuffer.RemoveData(datasize);
		m_RecvHeader.Reset();
	}

	int NetConnection::DoSend()
	{
		if (m_SendBuffer.GetLength() > 0)
		{
			int ret = ::send(GetSocket(), (char*)m_SendBuffer.GetData(), m_SendBuffer.GetLength(), 0);
			if (ret > 0)
			{
				m_SendBuffer.RemoveData(ret);
			}
			else if (ret == SOCKET_ERROR)
			{
				cout << "send socket err:" << GET_LAST_ERROR() << endl;
				return 1;
			}
		}

		return 0;
	}
}
