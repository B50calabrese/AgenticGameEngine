#ifndef ENGINE_SCRIPTING_SCRIPT_SYSTEM_H_
#define ENGINE_SCRIPTING_SCRIPT_SYSTEM_H_

#include "engine/ecs/registry.h"
#include "engine/scripting/lua_state.h"

namespace engine::scripting {

/**
 * @brief Updates ScriptComponent entities and hot-reloads modified Lua files.
 */
class ScriptSystem {
 public:
  static void Update(engine::ecs::Registry& registry, LuaState& lua, float dt);
};

}  // namespace engine::scripting

#endif  // ENGINE_SCRIPTING_SCRIPT_SYSTEM_H_
