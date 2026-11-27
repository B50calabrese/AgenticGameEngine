#include <gtest/gtest.h>

#include "engine/scripting/lua_state.h"

namespace engine::scripting {
namespace {

TEST(LuaStateTest, ExecuteStringAndReadGlobals) {
  LuaState lua;
  ASSERT_TRUE(lua.IsValid());

  std::string err;
  EXPECT_TRUE(lua.ExecuteString("score = 42; player = 'Hero'", &err)) << err;
  EXPECT_DOUBLE_EQ(lua.GetGlobalNumber("score"), 42.0);
  EXPECT_EQ(lua.GetGlobalString("player"), "Hero");
}

TEST(LuaStateTest, SyntaxErrorReturnsFalseAndMessage) {
  LuaState lua;
  std::string err;
  EXPECT_FALSE(lua.ExecuteString("local x = (", &err));
  EXPECT_FALSE(err.empty());
}

TEST(LuaStateTest, CallFunctionAndEngineBindings) {
  LuaState lua;
  lua.RegisterEngineBindings();

  std::string err;
  ASSERT_TRUE(lua.ExecuteString(R"(
    counter = 0
    function on_tick()
      counter = counter + 1
      engine.log("tick from lua")
    end
  )", &err)) << err;

  EXPECT_TRUE(lua.CallFunction("on_tick", &err)) << err;
  EXPECT_DOUBLE_EQ(lua.GetGlobalNumber("counter"), 1.0);
}

}  // namespace
}  // namespace engine::scripting
