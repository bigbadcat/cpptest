#ifndef __ModuleB_h__
#define __ModuleB_h__

#include <Macro.h>

namespace XX
{
	class ModuleB
	{
	public:

		//构造析构函数
		ModuleB(Int32 v);
		virtual ~ModuleB();

		void Fun();

	private:

		Byte m_Value[4];
	};
}

#endif