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
#include "NetConnection.h"
#include "NetConnectionManager.h"
#include "../Macro.h"
#include "../Config/LuaWrap.h"
#include "../MySQL/MySQLWrap.h"
#include "../tinyxml2/tinyxml2.h"
#include "../Util/DataUtil.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <assert.h>
using namespace std;
using namespace XX;

namespace Net
{
	//获取XML值
	string get_xml_value(tinyxml2::XMLNode *node, const string &key)
	{
		assert(node != NULL);
		tinyxml2::XMLElement *key_child = node->FirstChildElement(key.c_str());
		if (key_child != NULL)
		{
			tinyxml2::XMLNode *text_child = key_child->FirstChild();
			if (text_child != NULL)
			{
				tinyxml2::XMLText *text_node = text_child->ToText();
				if (text_node != NULL)
				{
					return string(text_node->Value());
				}
			}
		}
		return string();
	}

	NetServer::NetServer() : m_Port(0), m_MySQL(new MySQLWrap()), m_Run(false), m_pConnectionManager(NULL)
	{
		m_pConnectionManager = new NetConnectionManager();
		m_pConnectionManager->SetServer(this);
	}

	NetServer::~NetServer()
	{
		SAFE_DELETE(m_pConnectionManager);
		SAFE_DELETE(m_MySQL);
	}

	void NetServer::Init()
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError error = doc.LoadFile("StartSetting.xml");
		assert(error == tinyxml2::XMLError::XML_SUCCESS);
		tinyxml2::XMLElement *root = doc.FirstChildElement("StartSetting");
		for (tinyxml2::XMLElement *itr = root->FirstChildElement(); itr != NULL; itr = itr->NextSiblingElement())
		{
			tinyxml2::XMLNode *node = itr;
			string name(node->Value());
			if (name.compare("Master") == 0)
			{
				short mport = (short)::atoi(get_xml_value(node, "port").c_str());
				::printf("Master port:%d\n", mport);
			}
		}

		m_MySQL->Init("110.43.46.109", "root", "xx20200808", "xx004", 3306);
	}

	void NetServer::Start(int port)
	{
		m_Port = port;
		LuaWrap::GetLuaState();
		::printf("NetServer::Start port:%d\n", port);

		const char *sql = "select * from tb_server where id = 10001";
		auto_ptr<MySQLResult> result = m_MySQL->Query(sql);
		if (result->GetRecord())
		{
			string id = result->GetString("ip");
			int pt = result->GetInt("port");
			m_Port = pt;
			printf("server id:10001 ip:%s port:%d\n", id.c_str(), pt);
		}

		m_Run = true;
		m_Thread = thread([](NetServer *t){t->ThreadProcess(); }, this);
	}

	void NetServer::ThreadProcess()
	{
		m_pConnectionManager->Init(m_Port);

		std::chrono::milliseconds dura(200);
		while (m_Run)
		{
			m_pConnectionManager->SelectSocket(200);
			//std::this_thread::sleep_for(dura);
		}
		cout << "m_Run:" << m_Run << endl;

		m_pConnectionManager->Release();
	}

	void NetServer::Stop()
	{
		::printf("NetServer::Stop\n");
		m_Run = false;
		if (m_Thread.joinable())
		{
			m_Thread.join();
		}
		LuaWrap::ReleaseLua();
	}

	void NetServer::OnRecvData(NetConnection* con)
	{
		static Byte buffer[NET_PACKAGE_MAX_SIZE];
		NetPackageHeader header;
		assert(con != NULL);
		con->TakeRecvPackage(header, buffer, NET_PACKAGE_MAX_SIZE);

		//::printf("NetServer::OnRecvData con:%I64d cmd:%d\n", con->GetUniqueID(), header.Command);
		if (header.Command == 3)		//心跳请求
		{

		}
		else if(header.Command == 1001)		//用户登陆请求
		{
			int index = 0;
			string token = DataUtil::ReadString(buffer, index, &index);
			printf("login token:%s\n", token.c_str());

			int res_index = 0;
			Byte res_buffer[NET_PACKAGE_MAX_SIZE];
			res_index = DataUtil::WriteInt32(res_buffer, res_index, 0);
			res_index = DataUtil::WriteInt32(res_buffer, res_index, 0);
			con->Send(0, 1401, res_buffer, res_index);
		}
	}
}
