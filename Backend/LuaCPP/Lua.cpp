#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include "Backend/LuaCPP/Lua.h"

Lua::Lua()
	: m_state(luaL_newstate())
{
}

Lua::~Lua()
{
	if (IsValid())
	{
		lua_close(m_state);
	}
}

bool Lua::IsValid() const
{
	return m_state != nullptr;
}

Lua::Result Lua::DoString(ConstScriptRef command)
{
	return luaL_dostring(m_state, command.data());
}

Lua::Value Lua::GetGlobalValue(ConstScriptRef name)
{
	lua_getglobal(m_state, name.data());
	if (lua_isnumber(m_state, -1))
	{
		return Value(static_cast<double>(lua_tonumber(m_state, -1)));
	}
	else if (lua_isstring(m_state, -1))
	{
		return Value(std::string(lua_tostring(m_state, -1)));
	}

	DEBUG_BREAK(); // Tried to get a non-number, non-string lua global. Possibly trying to get an object?
	return Value();
}

#define IMPL_NUM_GetGlobalValueOfType(type) template <> DLLMODE type Lua::GetGlobalValueOfType<type>(ConstScriptRef name) { return GetGlobalNumberOfType<type>(name); }
IMPL_NUM_GetGlobalValueOfType(unsigned char)
IMPL_NUM_GetGlobalValueOfType(unsigned short)
IMPL_NUM_GetGlobalValueOfType(unsigned int)
IMPL_NUM_GetGlobalValueOfType(unsigned long)
IMPL_NUM_GetGlobalValueOfType(unsigned long long)
IMPL_NUM_GetGlobalValueOfType(char)
IMPL_NUM_GetGlobalValueOfType(short)
IMPL_NUM_GetGlobalValueOfType(int)
IMPL_NUM_GetGlobalValueOfType(long)
IMPL_NUM_GetGlobalValueOfType(long long)
IMPL_NUM_GetGlobalValueOfType(float)
IMPL_NUM_GetGlobalValueOfType(double)
IMPL_NUM_GetGlobalValueOfType(long double)
#undef IMPL_NUM_GetGlobalValueOfType

template <>
std::string Lua::GetGlobalValueOfType<std::string>(ConstScriptRef name)
{
	lua_getglobal(m_state, name.data());
	if (lua_isstring(m_state, -1))
	{
		return std::string(lua_tostring(m_state, -1));
	}
	else
	{
		DEBUG_BREAK(); // Tried to get a non-string lua global.
		return std::string();
	}
}
