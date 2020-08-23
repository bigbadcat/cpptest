/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetListener.cpp
* Summary: 网络通信模块服务端连接监听线程。
*
* Author: XuXiang
* Date: 2018-07-30 22:58
*******************************************************/

#include "NetListener.h"
#include "NetConnectionManager.h"
#include "NetDefine.h"
#include <assert.h>
#include <iostream>
using namespace std;

namespace Net
{
	NetListener::NetListener(): m_Socket(SOCKET_ERROR), m_Port(0), m_pManager(NULL)
	{
	}

	NetListener::~NetListener()
	{
		SAFE_CLOSE_SOCKET(m_Socket)
	}

	void NetListener::Start(int port)
	{
		cout << "NetListener::Start port:" << port << endl;
		m_Port = port;
		assert(m_Port > 0);
		assert(m_Socket == SOCKET_ERROR);
		m_Socket = CreateListenSocket();
		assert(m_Socket != SOCKET_ERROR);
	}

	void NetListener::Stop()
	{
		cout << "NetListener::Stop" << endl;
		SAFE_CLOSE_SOCKET(m_Socket)
	}

	socket_t NetListener::CreateListenSocket()
	{
		//创建socket开始监听
		socket_t s = ::socket(AF_INET, SOCK_STREAM, 0);
		if (s == SOCKET_ERROR)
		{
			cout << "create socket err:" << GET_LAST_ERROR() << endl;
			return SOCKET_ERROR;
		}

		//开始绑定
		sockaddr_in addr_in;
		::memset(&addr_in, 0, sizeof(addr_in));
		addr_in.sin_family = AF_INET;
		addr_in.sin_addr.s_addr = ::htonl(INADDR_ANY);
		addr_in.sin_port = ::htons(m_Port);
		int ret = ::bind(s, (const sockaddr *)&addr_in, sizeof(addr_in));
		if (ret != 0)
		{
			cout << "bind socket err:" << GET_LAST_ERROR() << endl;
			SAFE_CLOSE_SOCKET(s);
			return SOCKET_ERROR;
		}

		//开始监听
		ret = ::listen(s, 64);
		if (ret != 0)
		{
			cout << "listen socket err:" << GET_LAST_ERROR() << endl;
			SAFE_CLOSE_SOCKET(s);
			return SOCKET_ERROR;
		}

		//非阻塞
		if (!socket_set_nonblocking(s, true))
		{
			cout << "set listen socket noblocking err:" << GET_LAST_ERROR() << endl;
			SAFE_CLOSE_SOCKET(s);
			return SOCKET_ERROR;
		}
		return s;
	}

	int NetListener::OnSocketRead()
	{
		sockaddr_in r_addr;
		socklen_t addr_size = sizeof(r_addr);
		::memset(&r_addr, 0, sizeof(r_addr));
		socket_t rs = ::accept(m_Socket, (sockaddr*)&r_addr, &addr_size);
		if (rs == SOCKET_ERROR)
		{
			cout << "accept socket err:" << GET_LAST_ERROR() << endl;
			return 1;
		}

		//非阻塞
		if(!socket_set_nonblocking(rs, true))
		{
			cout << "set remote socket noblocking err:" << GET_LAST_ERROR() << endl;
			SAFE_CLOSE_SOCKET(rs);
			return 2;
		}

		linger so_linger;
		so_linger.l_onoff = 1;
		so_linger.l_linger = 5;
		int ret = ::setsockopt(rs, SOL_SOCKET, SO_LINGER, (char*)&so_linger, sizeof so_linger);
		if (ret != 0)
		{
			cout << "setsockopt err:" << GET_LAST_ERROR() << endl;
			SAFE_CLOSE_SOCKET(rs);
			return 3;
		}

		if (m_pManager != NULL)
		{
			m_pManager->AddConnection(rs);
		}

		return 0;
	}

	void NetListener::OnSocketClose()
	{
		SAFE_CLOSE_SOCKET(m_Socket)
	}
}
