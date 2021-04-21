#include <gtest/gtest.h>
#include "Backend/LuaCPP/Lua.h"

class LuaTests : public testing::Test
{
  void SetUp() override
  {
  }

  void TearDown() override
  {
  }

public:
  Lua lua;
};

TEST_F(LuaTests, TestAssignment)
{
  // given
  constexpr const char* luaScript = R"lua(a = 12)lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
  int a = lua.GetGlobalValueOfType<int>("a");
  EXPECT_EQ(a, 12);
}

TEST_F(LuaTests, TestAddition)
{
  // given
  constexpr const char* luaScript = R"lua(a = 5 + 10)lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
  int a = lua.GetGlobalValueOfType<int>("a");
  EXPECT_EQ(a, 15);
}

TEST_F(LuaTests, TestFunction)
{
  // given
  constexpr const char* luaScript =
    R"lua(
function multiply(lhs, rhs)
  return lhs * rhs
end

a = multiply(5, 3)
b = multiply(2.1, 10)
c = multiply(-4, 6)
)lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
  double a = lua.GetGlobalValueOfType<double>("a");
  EXPECT_DOUBLE_EQ(a, 15.0);
  double b = lua.GetGlobalValueOfType<double>("b");
  EXPECT_DOUBLE_EQ(b, 21.0);
  double c = lua.GetGlobalValueOfType<double>("c");
  EXPECT_DOUBLE_EQ(c, -24.0);
}

TEST_F(LuaTests, TestOSLibrary)
{
  // given
  constexpr const char* luaScript = R"lua(os.clock())lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
}

TEST_F(LuaTests, TestRequire)
{
  // given
  constexpr const char* luaScript = R"lua(local socket = require("socket"))lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
}

TEST_F(LuaTests, TestHTTP)
{
  // given
  constexpr const char* luaScript =
R"lua(
local http = require("socket.http")

local data = ""

local function collect(chunk)
  if chunk ~= nil then
    data = data .. chunk
    end
  return true
end

local ok, statusCode, headers, statusText = http.request {
  method = "GET",
  url = "http://google.com/",
  sink = collect
}

print("ok\t",         ok);
print("statusCode", statusCode)
print("statusText", statusText)
print("headers:")
for i,v in pairs(headers) do
  print("\t",i, v)
end
)lua";

  // when
  Lua::Result result = lua.DoString(luaScript);

  // expect
  EXPECT_EQ(result, LUA_OK);
}
