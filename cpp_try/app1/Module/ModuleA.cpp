#include <iostream>
#include "ModuleA.h"

namespace XX
{

	ModuleA::ModuleA(int v) : m_Value(v)
	{
	}

	ModuleA::~ModuleA()
	{
	}

	void ModuleA::Fun()
	{
		printf_s("ModuleA.Value:%d\n", m_Value);
	}
}