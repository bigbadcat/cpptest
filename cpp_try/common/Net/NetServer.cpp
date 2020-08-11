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
#include "NetListener.h"
#include "../Macro.h"
#include "../Config/LuaWrap.h"
#include "../MySQL/MySQLWrap.h"
#include "../tinyxml2/tinyxml2.h"
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

	NetServer::NetServer() : m_Port(0), m_MySQL(new MySQLWrap()), m_Socket(SOCKET_ERROR), m_Run(false), m_pListener(NULL)
	{
		m_pListener = new NetListener();
		m_pListener->SetServer(this);
	}

	NetServer::~NetServer()
	{
		SAFE_DELETE(m_pListener);
		SAFE_DELETE(m_MySQL);
		SAFE_CLOSE_SOCKET(m_Socket);
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
			printf("server id:10001 ip:%s port:%d\n", id.c_str(), pt);
		}

		m_Run = true;
		m_Thread = thread([](NetServer *t){t->ThreadProcess(); }, this);
	}

	void NetServer::ThreadProcess()
	{
		std::chrono::milliseconds dura(200);
		while (m_Run)
		{
			std::this_thread::sleep_for(dura);
		}
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

	void NetServer::AddConnection(socket_t s)
	{
		SAFE_CLOSE_SOCKET(m_Socket);
		m_Socket = s;
		::printf("NetServer::AddConnection socket:%ld\n", (long)m_Socket);
	}
}
