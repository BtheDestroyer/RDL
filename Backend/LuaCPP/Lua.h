#pragma once
#include "Backend/pch.h"
#include "Backend/Framework/Debug.h"
#include <string_view>
#include <any>
#include <lua/lua.hpp>

class DLLMODE Lua
{
public:
	using Script = std::string;
	using ConstScriptRef = const std::string_view&;
	using Value = std::any;
	using Result = int;
		
	Lua();
	Lua(const Lua&) = delete;
	Lua(Lua&&) = delete;
	Lua& operator=(const Lua&) = delete;
	Lua& operator=(Lua&&) = delete;
	~Lua();

	bool IsValid() const;

	[[nodiscard]] Result DoString(ConstScriptRef command);
	[[nodiscard]] Value GetGlobalValue(ConstScriptRef name);
	template <typename T> // This is explicitly specialized within the CPP
	[[nodiscard]] T GetGlobalValueOfType(ConstScriptRef name);
	template <typename T>
	[[nodiscard]] T GetGlobalNumberOfType(ConstScriptRef name)
	{
		lua_getglobal(m_state, name.data());
		if (lua_isnumber(m_state, -1))
		{
			return static_cast<T>(lua_tonumber(m_state, -1));
		}
		else
		{
			DEBUG_BREAK(); // Tried to get a non-number lua global.
			return 0;
		}
	}

private:
	lua_State* m_state;
};
