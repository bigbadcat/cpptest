#include <iostream>
#include <Net/NetServer.h>
#include "Module/ModuleA.h"
#include "Module/ModuleB/ModuleB.h"
using namespace Net;
using namespace XX;

int main()
{
	NetServer net;
	net.Init();
	net.Start(30);

	ModuleA ma(2);
	ma.Fun();

	ModuleB mb(10);
	mb.Fun();

	net.Stop();

#if defined(WIN)
	system("pause");
#endif
	return 0;
}