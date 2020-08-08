/*******************************************************
* Copyright (c) 2018-2088, By XuXiang all rights reserved.
*
* FileName: LuaWrap.cpp
* Summary: Lua配置基类。
*
* Author: XuXiang
* Date: 2019-01-24 20:57
*******************************************************/

#include "LuaWrap.h"
#include <lua.hpp>
#include <iostream>
#include <assert.h>

namespace XX
{
	//利用静态对象的析构函数自动销毁Lua
	class LuaDestructor
	{
	private:
		~LuaDestructor()
		{
			LuaWrap::ReleaseLua();
		}

		static LuaDestructor Destructor;
	};
	LuaDestructor LuaDestructor::Destructor;


	lua_State* LuaWrap::LuaState = NULL;

	LuaWrap::LuaWrap()
	{
	}

	LuaWrap::~LuaWrap()
	{
	}

	lua_State* LuaWrap::GetLuaState()
	{
		static bool init = false;
		if (!init)
		{
			init = true;
			LuaState = luaL_newstate();
			luaL_openlibs(LuaState);
			DoLuaFile(LuaState, "Lua/Main.lua");
		}

		return LuaState;
	}

	void LuaWrap::ReleaseLua()
	{
		if (LuaState == NULL)
		{
			return;
		}
		lua_close(LuaState);
		LuaState = NULL;
	}

	bool LuaWrap::DoLuaFile(lua_State *L, const char *file, bool resetstack)
	{
		assert(L != NULL);
		int top = lua_gettop(L);
		int err = luaL_dofile(L, file);
		if (err != LUA_OK)
		{
			const char *msg = lua_tostring(L, -1);
			if (msg != NULL)
			{
				printf("Lua error!\n%s\n", lua_tostring(L, -1));
			}
		}
		if (resetstack)
		{
			lua_settop(L, top);
		}		
		return err == LUA_OK;
	}

	bool LuaWrap::ReadBool(lua_State *L, char *name)
	{
		lua_pushstring(L, name);
		lua_gettable(L, -2);
		bool ret = lua_toboolean(L, -1) != 0;
		lua_pop(L, 1);
		return ret;
	}

	void LuaWrap::ReadBoolArray(lua_State *L, char *name, vector<bool> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			bool value = lua_toboolean(L, -1) != 0;
			v.push_back(value);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	int LuaWrap::ReadInt(lua_State *L, char *name)
	{
		return (int)ReadDouble(L, name);
	}

	void LuaWrap::ReadIntArray(lua_State *L, char *name, vector<int> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			int value = (int)lua_tonumber(L, -1);
			v.push_back(value);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	Int64 LuaWrap::ReadLong(lua_State *L, char *name)
	{
		return (Int64)ReadDouble(L, name);
	}

	void LuaWrap::ReadLongArray(lua_State *L, char *name, vector<Int64> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			Int64 value = (Int64)lua_tonumber(L, -1);
			v.push_back(value);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	float LuaWrap::ReadFloat(lua_State *L, char *name)
	{
		return (float)ReadDouble(L, name);
	}

	void LuaWrap::ReadFloatArray(lua_State *L, char *name, vector<float> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			float value = (float)lua_tonumber(L, -1);
			v.push_back(value);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	double LuaWrap::ReadDouble(lua_State *L, char *name)
	{
		lua_pushstring(L, name);
		lua_gettable(L, -2);
		double ret = lua_tonumber(L, -1);
		lua_pop(L, 1);
		return ret;
	}

	void LuaWrap::ReadDoubleArray(lua_State *L, char *name, vector<double> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			double value = (double)lua_tonumber(L, -1);
			v.push_back(value);
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}

	string LuaWrap::ReadString(lua_State *L, char *name)
	{
		string str;
		ReadString(L, name, str);
		return str;
	}

	void LuaWrap::ReadString(lua_State *L, char *name, string &str)
	{
		lua_pushstring(L, name);
		lua_gettable(L, -2);
		const char *ret = lua_tostring(L, -1);
		lua_pop(L, 1);
		str.assign(ret);
	}

	void LuaWrap::ReadStringArray(lua_State *L, char *name, vector<string> &v)
	{
		v.clear();
		lua_pushstring(L, name);
		lua_gettable(L, -2);

		lua_pushnil(L);
		while (lua_next(L, -2))		//堆栈状态 -1:value -2:key -3:array(table)
		{
			const char *value = lua_tostring(L, -1);
			v.push_back(string(value));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
}