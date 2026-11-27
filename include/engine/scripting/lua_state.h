#ifndef ENGINE_SCRIPTING_LUA_STATE_H_
#define ENGINE_SCRIPTING_LUA_STATE_H_

#include <string>

struct lua_State;

namespace engine::scripting {

/**
 * @brief RAII wrapper around a Lua 5.4 VM with engine C++ bindings.
 */
class LuaState {
 public:
  LuaState();
  ~LuaState();

  LuaState(const LuaState&) = delete;
  LuaState& operator=(const LuaState&) = delete;
  LuaState(LuaState&& other) noexcept;
  LuaState& operator=(LuaState&& other) noexcept;

  bool ExecuteString(const std::string& code,
                     std::string* out_error = nullptr);
  bool ExecuteFile(const std::string& path, std::string* out_error = nullptr);
  bool CallFunction(const std::string& name,
                    std::string* out_error = nullptr);

  void RegisterEngineBindings();

  double GetGlobalNumber(const std::string& name,
                         double default_val = 0.0) const;
  std::string GetGlobalString(const std::string& name,
                              const std::string& default_val = "") const;

  bool IsValid() const { return state_ != nullptr; }

 private:
  lua_State* state_ = nullptr;
};

}  // namespace engine::scripting

#endif  // ENGINE_SCRIPTING_LUA_STATE_H_
