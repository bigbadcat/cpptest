#include <iostream>
#include "Module/ModuleA.h"
using namespace XX;

int main()
{
	ModuleA m(2);
	m.Fun();
#if defined(WIN)
	system("pause");
#endif
	return 0;
}