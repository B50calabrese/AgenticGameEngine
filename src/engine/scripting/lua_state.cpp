#include "engine/scripting/lua_state.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include "engine/core/log.h"
#include "engine/input/input.h"
#include "engine/renderer/color.h"
#include "engine/renderer/primitive_renderer.h"

namespace engine::scripting {

namespace {

int LuaLog(lua_State* L) {
  const char* msg = luaL_checkstring(L, 1);
  ENGINE_LOG_INFO(std::string("[Lua] ") + msg);
  return 0;
}

int LuaDrawQuad(lua_State* L) {
  const float x = static_cast<float>(luaL_checknumber(L, 1));
  const float y = static_cast<float>(luaL_checknumber(L, 2));
  const float w = static_cast<float>(luaL_checknumber(L, 3));
  const float h = static_cast<float>(luaL_checknumber(L, 4));
  const float r = static_cast<float>(luaL_optnumber(L, 5, 1.0));
  const float g = static_cast<float>(luaL_optnumber(L, 6, 1.0));
  const float b = static_cast<float>(luaL_optnumber(L, 7, 1.0));
  const float a = static_cast<float>(luaL_optnumber(L, 8, 1.0));
  engine::renderer::PrimitiveRenderer::DrawQuad(
      x, y, w, h, engine::renderer::Color(r, g, b, a));
  return 0;
}

int LuaDrawCircle(lua_State* L) {
  const float x = static_cast<float>(luaL_checknumber(L, 1));
  const float y = static_cast<float>(luaL_checknumber(L, 2));
  const float radius = static_cast<float>(luaL_checknumber(L, 3));
  const float r = static_cast<float>(luaL_optnumber(L, 4, 1.0));
  const float g = static_cast<float>(luaL_optnumber(L, 5, 1.0));
  const float b = static_cast<float>(luaL_optnumber(L, 6, 1.0));
  const float a = static_cast<float>(luaL_optnumber(L, 7, 1.0));
  engine::renderer::PrimitiveRenderer::DrawCircle(
      x, y, radius, engine::renderer::Color(r, g, b, a));
  return 0;
}

int LuaIsKeyPressed(lua_State* L) {
  const int key = static_cast<int>(luaL_checkinteger(L, 1));
  const bool pressed = engine::input::Input::IsKeyPressed(
      static_cast<engine::input::KeyCode>(key));
  lua_pushboolean(L, pressed ? 1 : 0);
  return 1;
}

}  // namespace

LuaState::LuaState() {
  state_ = luaL_newstate();
  if (state_ != nullptr) {
    luaL_openlibs(state_);
  }
}

LuaState::~LuaState() {
  if (state_ != nullptr) {
    lua_close(state_);
    state_ = nullptr;
  }
}

LuaState::LuaState(LuaState&& other) noexcept : state_(other.state_) {
  other.state_ = nullptr;
}

LuaState& LuaState::operator=(LuaState&& other) noexcept {
  if (this != &other) {
    if (state_ != nullptr) {
      lua_close(state_);
    }
    state_ = other.state_;
    other.state_ = nullptr;
  }
  return *this;
}

bool LuaState::ExecuteString(const std::string& code, std::string* out_error) {
  if (state_ == nullptr) {
    if (out_error != nullptr) {
      *out_error = "LuaState is null";
    }
    return false;
  }
  if (luaL_dostring(state_, code.c_str()) != LUA_OK) {
    if (out_error != nullptr) {
      *out_error = lua_tostring(state_, -1);
    }
    lua_pop(state_, 1);
    return false;
  }
  return true;
}

bool LuaState::ExecuteFile(const std::string& path, std::string* out_error) {
  if (state_ == nullptr) {
    if (out_error != nullptr) {
      *out_error = "LuaState is null";
    }
    return false;
  }
  if (luaL_dofile(state_, path.c_str()) != LUA_OK) {
    if (out_error != nullptr) {
      *out_error = lua_tostring(state_, -1);
    }
    lua_pop(state_, 1);
    return false;
  }
  return true;
}

bool LuaState::CallFunction(const std::string& name, std::string* out_error) {
  if (state_ == nullptr) {
    if (out_error != nullptr) {
      *out_error = "LuaState is null";
    }
    return false;
  }
  lua_getglobal(state_, name.c_str());
  if (!lua_isfunction(state_, -1)) {
    lua_pop(state_, 1);
    if (out_error != nullptr) {
      *out_error = "Function not found: " + name;
    }
    return false;
  }
  if (lua_pcall(state_, 0, 0, 0) != LUA_OK) {
    if (out_error != nullptr) {
      *out_error = lua_tostring(state_, -1);
    }
    lua_pop(state_, 1);
    return false;
  }
  return true;
}

void LuaState::RegisterEngineBindings() {
  if (state_ == nullptr) {
    return;
  }
  lua_newtable(state_);

  lua_pushcfunction(state_, LuaLog);
  lua_setfield(state_, -2, "log");

  lua_pushcfunction(state_, LuaDrawQuad);
  lua_setfield(state_, -2, "draw_quad");

  lua_pushcfunction(state_, LuaDrawCircle);
  lua_setfield(state_, -2, "draw_circle");

  lua_pushcfunction(state_, LuaIsKeyPressed);
  lua_setfield(state_, -2, "is_key_pressed");

  lua_setglobal(state_, "engine");
}

double LuaState::GetGlobalNumber(const std::string& name,
                                 double default_val) const {
  if (state_ == nullptr) {
    return default_val;
  }
  lua_getglobal(state_, name.c_str());
  double result = default_val;
  if (lua_isnumber(state_, -1)) {
    result = lua_tonumber(state_, -1);
  }
  lua_pop(state_, 1);
  return result;
}

std::string LuaState::GetGlobalString(const std::string& name,
                                      const std::string& default_val) const {
  if (state_ == nullptr) {
    return default_val;
  }
  lua_getglobal(state_, name.c_str());
  std::string result = default_val;
  if (lua_isstring(state_, -1)) {
    result = lua_tostring(state_, -1);
  }
  lua_pop(state_, 1);
  return result;
}

}  // namespace engine::scripting
