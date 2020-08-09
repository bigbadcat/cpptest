#include <iostream>
#include <Net/NetServer.h>
#include "Module/ModuleA.h"
#include "Module/ModuleB/ModuleB.h"

#if defined(WIN)
#include <WinSock2.h>
#endif

using namespace Net;
using namespace XX;

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

	net.Stop();

#if defined(WIN)
	::WSACleanup();
	system("pause");
#endif
	return 0;
}