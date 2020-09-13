#ifndef __HttpTest_h__
#define __HttpTest_h__

#include <Http/HttpServer.h>

namespace XX
{
#define HTTP_SERVER_REGISTER(path, fun) \
	RegisterServer(path, [this](const string &p, HttpParamMap &params, HttpResponse &res){this->fun(p, params, res); })

	class HttpTest : public HttpServer
	{
	public:

		//构造析构函数
		HttpTest();
		virtual ~HttpTest();

	protected:
		virtual void OnRegisterServer();

		//初始化
		virtual void OnInit();

		//更新
		virtual void OnUpdate();

		//释放
		virtual void OnRelease();

	private:

		//登陆请求
		void OnTestRequest(const string &path, HttpParamMap &params, HttpResponse &res);
	};
}

#endif
