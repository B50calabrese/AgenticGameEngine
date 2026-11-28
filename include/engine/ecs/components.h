#ifndef ENGINE_ECS_COMPONENTS_H_
#define ENGINE_ECS_COMPONENTS_H_

#include <cstdint>
#include <memory>
#include <string>

#include "engine/math/vector2.h"
#include "engine/renderer/color.h"
#include "engine/renderer/texture.h"

namespace engine::ecs {

struct TagComponent {
  std::string tag;
};

struct Transform2DComponent {
  engine::math::Vector2 position{0.0f, 0.0f};
  engine::math::Vector2 scale{1.0f, 1.0f};
  float rotation_radians = 0.0f;
};

struct SpriteComponent {
  engine::math::Vector2 size{32.0f, 32.0f};
  engine::renderer::Color color = engine::renderer::Color::White();
  std::shared_ptr<engine::renderer::Texture2D> texture = nullptr;
};

struct ScriptComponent {
  std::string script_path;
  bool initialized = false;
  std::int64_t last_write_time = 0;
};

}  // namespace engine::ecs

#endif  // ENGINE_ECS_COMPONENTS_H_
