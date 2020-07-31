#ifndef __ModuleA_h__
#define __ModuleA_h__

namespace XX
{
	class ModuleA
	{
	public:

		//构造析构函数
		ModuleA(int v);
		virtual ~ModuleA();

		void Fun();

	private:

		int m_Value;
	};
}

#endif
