/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: HttpServer.cpp
* Summary: Http服务基类。
*
* Author: XuXiang
* Date: 2019-09-27 17:07
*******************************************************/

#include "HttpServer.h"
#include <assert.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/buffer.h>
#include "../Util/StringUtil.h"

namespace XX
{
	HttpResponse::HttpResponse() : m_ResBuff(NULL)
	{
		m_ResBuff = evbuffer_new();
	}

	HttpResponse::~HttpResponse()
	{
		if (m_ResBuff != NULL)
		{
			evbuffer_free(m_ResBuff);
			m_ResBuff = NULL;
		}
	}

	void HttpResponse::AddText(const char *fmt, ...)
	{
		va_list ap;
		va_start(ap, fmt);	//获得可变参数列表,分析fmt，得出各个参数的类型
		evbuffer_add_vprintf(m_ResBuff, fmt, ap);
		va_end(ap);			//释放资源,必须与va_start成对调用
	}

	void HttpResponse::SendReply(struct evhttp_request *req)
	{
		evhttp_send_reply(req, HTTP_OK, NULL, m_ResBuff);
	}

	void HttpResponse::AddLuaKey(const char *key)
	{
		assert(m_LuaTableKeyCount.size() > 0);
		if (AddLuaKeyCount() > 0)
		{
			AddText(", ");
		}
		if (key != NULL)
		{
			AddText("%s=", key);
		}		
	}
	
	void HttpResponse::BeginLuaTable()
	{
		AddText ("{");
		m_LuaTableKeyCount.push(0);
	}

	void HttpResponse::EndLuaTable()
	{
		m_LuaTableKeyCount.pop();
		AddText ("}");
	}

	void HttpResponse::AddLuaKeyValue(const char *key, int value)
	{
		AddLuaKey(key);
		AddText("%d", value);
	}

	void HttpResponse::AddLuaKeyValue(const char *key, Int64 value)
	{
		AddLuaKey(key);
		AddText("%I64d", value);
	}

	void HttpResponse::AddLuaKeyValue(const char *key, const char *value)
	{
		AddLuaKey(key);
		AddText("\"%s\"", value);
	}

	int HttpResponse::AddLuaKeyCount()
	{
		int n = m_LuaTableKeyCount.top();
		m_LuaTableKeyCount.pop();
		m_LuaTableKeyCount.push(n + 1);
		return n;
	}

	HttpServer::HttpServer() : m_Run(false), m_Port(0), m_EventBase(NULL)
	{
	}

	HttpServer::~HttpServer()
	{
		for (HttpRequestMap::iterator itr = m_HttpRequest.begin(); itr != m_HttpRequest.end(); ++itr)
		{
			evhttp_connection_free(itr->second);
			itr->second = NULL;
		}
		m_HttpRequest.clear();

		if (m_EventBase != NULL)
		{
			event_base_free(m_EventBase);
			m_EventBase = NULL;
		}
	}

	void HttpServer::Start(short port)
	{
		m_Port = port;
		m_Run = true;
		m_Thread = thread([](HttpServer *t){t->ThreadProcess(); }, this);
	}

	void HttpServer::Stop()
	{
		if (m_Run)
		{
			//m_EventBase很大可能处于阻塞状态，没开启多线程模式(VLD检测内存泄漏)是不会唤醒，也就不会退出事件循环
			//手动触发个事件将event_base线程唤醒以退出事件循环
			m_Run = false;
			event_base_loopbreak(m_EventBase);
			//SendHttpRequest("127.0.0.1", m_Port, "/empty", HttpParamMap());

			//等待结束
			if (m_Thread.joinable())
			{
				m_Thread.join();
			}
		}
	}

	string HttpServer::GetHttpParams(const string &uri, HttpParamMap &params)
	{
		//通过?确定路径和参数部分
		size_t pos = uri.find_first_of('?');
		params.clear();
		if (pos == string::npos)
		{
			return uri;
		}
		string path = uri.substr(0, pos);
		string p_str = uri.substr(pos + 1);
	
		//解析参数
		vector<string> v;
		StringUtil::Split(p_str, "&", v);
		for (vector<string>::iterator itr = v.begin(); itr != v.end(); ++itr)
		{
			size_t p = itr->find_first_of('=');
			string key = itr->substr(0, p);
			string value = itr->substr(p+1);
			params[key] = value;
		}
		return path;
	}

	string HttpServer::GetHttpURI(const string &path, HttpParamMap &params)
	{
		string uri(path);
		if (params.size () > 0)
		{
			int n = 0;
			uri.append("?");
			for (HttpParamMap::iterator itr = params.begin(); itr != params.end(); ++itr)
			{
				if (n > 0)
				{
					uri.append("&");
				}
				++n;
				uri.append(itr->first);
				uri.append("=");
				uri.append(itr->second);
			}
		}
		return uri;
	}

	void HttpServer::SendHttpRequest(const string& ip, int port, const string& path, HttpParamMap &params)
	{
		if (m_EventBase == NULL)
		{
			return;
		}

		string uri = GetHttpURI(path, params);
		struct evhttp_connection *con = evhttp_connection_base_new(m_EventBase, NULL, ip.c_str(), port);
		struct evhttp_request *req = evhttp_request_new(OnHttpRequestDone, this);
		m_HttpRequest.insert(HttpRequestMap::value_type(req, con));
		evhttp_connection_set_timeout(con, 10);
		evhttp_make_request(con, req, EVHTTP_REQ_GET, uri.c_str());
	}

	void HttpServer::RegisterServer(string path, HttpServerCallBack call)
	{
		m_ServerCallBack[path] = call;
	}

	void HttpServer::ThreadProcess()
	{
		if (m_EventBase == NULL)
		{
			m_EventBase = event_base_new();
		}
		printf("Using Libevent with backend method <%s>.\n", event_base_get_method(m_EventBase));	//获取当前使用了哪种I/O模型，Windows下仅显示win32
		
		//启动服务
		const char *http_addr = "0.0.0.0";
		struct evhttp *http_server = evhttp_new(m_EventBase);
		evhttp_bind_socket(http_server, http_addr, m_Port);
		evhttp_set_gencb(http_server, HttpServer::OnHttpRequest, this);
		OnRegisterServer();

		//处理请求
		OnInit();
		std::chrono::milliseconds dura(100);
		while (m_Run)
		{
			event_base_loop(m_EventBase, EVLOOP_NONBLOCK);
			OnUpdate();
			std::this_thread::sleep_for(dura);
		}
		//event_base_dispatch(m_EventBase);
		OnRelease();
		
		evhttp_free(http_server);
		http_server = NULL;
	}

	void HttpServer::OnHttpRequest(struct evhttp_request *req, void *arg)
	{
		//解析参数
		XX::HttpServer *pserver = (XX::HttpServer*)arg;
		string uri(evhttp_request_get_uri(req));
		HttpParamMap params_set;
		string path = GetHttpParams(uri, params_set);

		//分派服务
		HttpServerCallBackMap::iterator itr = pserver->m_ServerCallBack.find(path);
		if (itr != pserver->m_ServerCallBack.end())
		{
			//处理服务，回复结果
			HttpResponse res;
			itr->second(path, params_set, res);
			res.SendReply(req);
			return;
		}

		//默认回调
		if (pserver->m_DefaultCallBack != NULL)
		{
			HttpResponse res;
			pserver->m_DefaultCallBack(path, params_set, res);
			res.SendReply(req);
			return;
		}

		//回复错误
		struct evbuffer *retbuff = evbuffer_new();
		evbuffer_add_printf(retbuff, "The requested URL %s was not found on this server.", uri.c_str());
		evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", retbuff);
		evbuffer_free(retbuff);
		retbuff = NULL;
	}

	void HttpServer::OnHttpRequestDone(struct evhttp_request *req, void *arg)
	{
		//将请求相关的连接释放
		XX::HttpServer *pserver = (XX::HttpServer*)arg;
		HttpRequestMap::iterator itr = pserver->m_HttpRequest.find(req);
		if (itr == pserver->m_HttpRequest.end())
		{
			return;
		}
		evhttp_connection_free(itr->second);		//释放连接，则连接上的请求也会一并释放
		itr->second = NULL;
		itr = pserver->m_HttpRequest.erase(itr);
	}
}
