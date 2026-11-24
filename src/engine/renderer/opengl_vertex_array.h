#ifndef SRC_ENGINE_RENDERER_OPENGL_VERTEX_ARRAY_H_
#define SRC_ENGINE_RENDERER_OPENGL_VERTEX_ARRAY_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "engine/renderer/vertex_array.h"

namespace engine::renderer {

class OpenGLVertexArray : public VertexArray {
 public:
  OpenGLVertexArray();
  ~OpenGLVertexArray() override;

  void Bind() const override;
  void Unbind() const override;

  void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertex_buffer) override;
  void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& index_buffer) override;

  const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override {
    return vertex_buffers_;
  }

  const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override {
    return index_buffer_;
  }

  std::uint32_t GetRendererId() const { return renderer_id_; }

 private:
  std::uint32_t renderer_id_ = 0;
  std::uint32_t vertex_buffer_index_ = 0;
  std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
  std::shared_ptr<IndexBuffer> index_buffer_;
};

}  // namespace engine::renderer

#endif  // SRC_ENGINE_RENDERER_OPENGL_VERTEX_ARRAY_H_
