#ifndef ENGINE_RENDERER_VERTEX_ARRAY_H_
#define ENGINE_RENDERER_VERTEX_ARRAY_H_

#include <memory>
#include <vector>

#include "engine/renderer/buffer.h"

namespace engine::renderer {

/**
 * @brief Platform-independent interface for Vertex Array Objects (VAO).
 *
 * Encapsulates vertex buffer bindings, vertex attribute pointers,
 * and element index buffer state.
 */
class VertexArray {
 public:
  virtual ~VertexArray() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) = 0;
  virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) = 0;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

  /**
   * @brief Factory creating a platform-appropriate VertexArray instance.
   */
  static std::shared_ptr<VertexArray> Create();
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_VERTEX_ARRAY_H_
