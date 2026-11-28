#include "engine/scripting/script_system.h"

#include <filesystem>

#include "engine/core/log.h"
#include "engine/ecs/components.h"

namespace engine::scripting {

void ScriptSystem::Update(engine::ecs::Registry& registry, LuaState& lua,
                          float dt) {
  registry.GetView<engine::ecs::ScriptComponent>().Each(
      [&](engine::ecs::EntityId entity, engine::ecs::ScriptComponent& script) {
        if (script.script_path.empty() ||
            !std::filesystem::exists(script.script_path)) {
          return;
        }

        const auto ftime =
            std::filesystem::last_write_time(script.script_path);
        const std::int64_t mod_time = ftime.time_since_epoch().count();

        if (!script.initialized || mod_time != script.last_write_time) {
          std::string err;
          if (lua.ExecuteFile(script.script_path, &err)) {
            script.initialized = true;
            script.last_write_time = mod_time;
            lua.CallFunction("on_create");
          } else {
            ENGINE_LOG_ERROR(std::string("[ScriptSystem] ") + err);
            return;
          }
        }

        lua.ExecuteString("dt = " + std::to_string(dt) +
                          "; entity_id = " + std::to_string(entity));
        lua.CallFunction("on_update");
      });
}

}  // namespace engine::scripting
