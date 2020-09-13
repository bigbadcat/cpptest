#include <stdio.h>
#include "HttpTest.h"
#include <Util/TimeUtil.h>

namespace XX
{

	HttpTest::HttpTest()
	{
	}

	HttpTest::~HttpTest()
	{
	}

	void HttpTest::OnRegisterServer()
	{
		HTTP_SERVER_REGISTER("/http_test", OnTestRequest);
	}

	void HttpTest::OnInit()
	{
	}

	void HttpTest::OnUpdate()
	{
	}

	void HttpTest::OnRelease()
	{
	}

	void HttpTest::OnTestRequest(const string &path, HttpParamMap &params, HttpResponse &res)
	{
		string p = params["p"];
		printf_s("OnTestRequest cmd:%s p:%s\n", path.c_str(), p.c_str());

		Int64 time = TimeUtil::GetCurrentSecond();
		res.BeginLuaTable();
		res.AddLuaKeyValue("p", p.c_str());
		res.AddLuaKeyValue("time", time);
		res.EndLuaTable();
	}
}
