/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: NetServer.cpp
* Summary: 网络通信模块服务端。
*
* Author: XuXiang
* Date: 2018-07-30 22:56
*******************************************************/

#include "NetServer.h"
#include "../Macro.h"
#include <iostream>
#include <string>
#include <assert.h>

namespace Net
{
	NetServer::NetServer() : m_Port(0)
	{
	}

	NetServer::~NetServer()
	{
	}

	void NetServer::Start(int port)
	{
		m_Port = port;
		::printf("NetServer::Start port:%d\n", port);
	}

	void NetServer::Stop()
	{
		::printf("NetServer::Stop\n");
	}
}