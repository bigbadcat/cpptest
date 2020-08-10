#include <iostream>
#include <stdio.h>
#include <Net/NetServer.h>
#include "Module/ModuleA.h"
#include "Module/ModuleB/ModuleB.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <unistd.h>
#include<sys/types.h>
#include <sys/stat.h>

#if defined(WIN)
#include <WinSock2.h>
#endif

using namespace Net;
using namespace XX;
using namespace std;

bool is_run = false;

void OnSignal(int signum)
{
	printf("OnSignal %d\n", signum);
	is_run = false;
}

int main()
{

	//1. 创建子进程，每个进程都会返回一个数字，父进程返回子进程的pid，子进程返回0
	int ret_code = fork();
	if (ret_code < 0)
	{
		return 1;
	}
	else if (ret_code>0)
	{
		//父进程退出
		return 0;
	}
	
	//2. 令子进程与父进程的会话组和进程组脱离，令子进程不受终端关闭影响
	setsid();


	//5. 设置进程的文件操作权限，使进程拥有最大权限
	umask(0);
	
	//6. 将父进程打开的文件描述符关闭
	//getdtablesize():获取描述符表的大小
	for(int fd = 0;fd< getdtablesize();fd++)
	{
		close(fd);
	}


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
