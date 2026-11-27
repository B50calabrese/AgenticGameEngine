#ifndef ENGINE_RENDERER_RENDER_QUEUE_H_
#define ENGINE_RENDERER_RENDER_QUEUE_H_

#include <memory>
#include <vector>

#include "engine/math/matrix4.h"
#include "engine/math/vector2.h"
#include "engine/renderer/color.h"
#include "engine/renderer/texture.h"

namespace engine::renderer {

/**
 * @brief Z-sorted 2D render submission queue.
 *
 * Collects draw commands with arbitrary submission order, sorts them stably
 * by Z-layer (back-to-front), and flushes them through PrimitiveRenderer.
 */
class RenderQueue {
 public:
  void SubmitQuad(const engine::math::Vector2& position,
                  const engine::math::Vector2& size,
                  const Color& color, float z_index = 0.0f,
                  float rotation_radians = 0.0f);

  void SubmitTexturedQuad(const engine::math::Vector2& position,
                          const engine::math::Vector2& size,
                          const std::shared_ptr<Texture2D>& texture,
                          const Color& tint = Color::White(),
                          float z_index = 0.0f,
                          float rotation_radians = 0.0f);

  /**
   * @brief Sorts queued commands by Z-index and flushes them to PrimitiveRenderer.
   */
  void Flush(const engine::math::Matrix4& view_projection);

  void Clear();
  std::size_t GetCommandCount() const { return commands_.size(); }

 private:
  struct RenderCommandItem {
    engine::math::Vector2 position;
    engine::math::Vector2 size;
    Color color;
    std::shared_ptr<Texture2D> texture;
    float z_index = 0.0f;
    float rotation_radians = 0.0f;
  };

  std::vector<RenderCommandItem> commands_;
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_RENDER_QUEUE_H_
