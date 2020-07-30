#include <stdio.h>
#include "ModuleB.h"
#include "../../Util/DataUtil.h"

namespace XX
{

	ModuleB::ModuleB(Int32 v)
	{
		DataUtil::WriteInt32(m_Value, 0, v);
	}

	ModuleB::~ModuleB()
	{
	}

	void ModuleB::Fun()
	{
		int next = 0;
		Int32 v = DataUtil::ReadInt32(m_Value, 0, &next);
		printf("ModuleB.Value:%d\n", v);
	}
}
