#include <iostream>
#include <Net/NetServer.h>
#include "Module/ModuleA.h"
#include "Module/ModuleB/ModuleB.h"
#include <csignal>

#if defined(WIN)
#include <WinSock2.h>
#endif

using namespace Net;
using namespace XX;

bool is_run = false;

void OnSignal(int signum)
{
	printf("OnSignal %d\n", signum);
	is_run = false;
}

int main()
{
#if defined(WIN)
	//初始化网络
	WORD wVersionRequested;
	WSADATA wsaData;
	int32_t err;
	wVersionRequested = MAKEWORD(2, 2);
	err = ::WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		printf_s("WSAStartup err:%d\n", err);
		return 1;
	}

#endif

	NetServer net;
	net.Init();
	net.Start(30);

	ModuleA ma(2);
	ma.Fun();

	ModuleB mb(10);
	mb.Fun();

	is_run = true;
#if defined(WIN)
	printf_s("input /q to quit ...\n");
	char str[64];
	while (is_run)
	{
		//接收命令
		cin.getline(str, sizeof(str));

		//如果输入内容超过缓冲区
		if (!std::cin)
		{
			std::cin.clear(); // 清除错误标志位
			std::cin.sync(); // 清除流
		}

		//提交命令
		string cmd(str);
		if (cmd.compare("/q") == 0)
		{
			is_run = false;
		}
	}
#else
	//linux下走信号退出
	signal(SIGTERM, OnSignal);
	signal(SIGINT, OnSignal);
	std::chrono::milliseconds dura(200);
	while (is_run)
	{
		std::this_thread::sleep_for(dura);
	}
#endif

	net.Stop();

#if defined(WIN)
	::WSACleanup();
#endif
	return 0;
}