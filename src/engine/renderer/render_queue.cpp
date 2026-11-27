#include "engine/renderer/render_queue.h"

#include <algorithm>

#include "engine/renderer/primitive_renderer.h"

namespace engine::renderer {

void RenderQueue::SubmitQuad(const engine::math::Vector2& position,
                             const engine::math::Vector2& size,
                             const Color& color, float z_index,
                             float rotation_radians) {
  commands_.push_back({position, size, color, nullptr, z_index, rotation_radians});
}

void RenderQueue::SubmitTexturedQuad(const engine::math::Vector2& position,
                                     const engine::math::Vector2& size,
                                     const std::shared_ptr<Texture2D>& texture,
                                     const Color& tint, float z_index,
                                     float rotation_radians) {
  commands_.push_back({position, size, tint, texture, z_index, rotation_radians});
}

void RenderQueue::Flush(const engine::math::Matrix4& view_projection) {
  if (commands_.empty()) {
    return;
  }

  std::stable_sort(commands_.begin(), commands_.end(),
                   [](const RenderCommandItem& a, const RenderCommandItem& b) {
                     return a.z_index < b.z_index;
                   });

  PrimitiveRenderer::BeginBatch(view_projection);

  for (const auto& cmd : commands_) {
    if (cmd.texture) {
      PrimitiveRenderer::DrawTexturedQuad(cmd.position, cmd.size, cmd.texture,
                                          cmd.rotation_radians, cmd.color);
    } else if (cmd.rotation_radians != 0.0f) {
      PrimitiveRenderer::DrawRotatedQuad(cmd.position, cmd.size,
                                         cmd.rotation_radians, cmd.color);
    } else {
      PrimitiveRenderer::DrawQuad(cmd.position, cmd.size, cmd.color);
    }
  }

  PrimitiveRenderer::EndBatch();
  commands_.clear();
}

void RenderQueue::Clear() {
  commands_.clear();
}

}  // namespace engine::renderer
