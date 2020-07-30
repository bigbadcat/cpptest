#include <iostream>
#include "Module/ModuleA.h"
#include "Module/ModuleB/ModuleB.h"
using namespace XX;

int main()
{
	ModuleA ma(2);
	ma.Fun();

	ModuleB mb(10);
	mb.Fun();
#if defined(WIN)
	system("pause");
#endif
	return 0;
}