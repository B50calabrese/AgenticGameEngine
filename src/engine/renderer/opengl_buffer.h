#ifndef SRC_ENGINE_RENDERER_OPENGL_BUFFER_H_
#define SRC_ENGINE_RENDERER_OPENGL_BUFFER_H_

#include <cstdint>

#include "engine/renderer/buffer.h"

namespace engine::renderer {

class OpenGLVertexBuffer : public VertexBuffer {
 public:
  explicit OpenGLVertexBuffer(std::uint32_t size);
  OpenGLVertexBuffer(const void* vertices, std::uint32_t size);
  ~OpenGLVertexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  void SetData(const void* data, std::uint32_t size) override;

  const BufferLayout& GetLayout() const override { return layout_; }
  void SetLayout(const BufferLayout& layout) override { layout_ = layout; }

  std::uint32_t GetRendererId() const { return renderer_id_; }

 private:
  std::uint32_t renderer_id_ = 0;
  BufferLayout layout_;
};

class OpenGLIndexBuffer : public IndexBuffer {
 public:
  OpenGLIndexBuffer(const std::uint32_t* indices, std::uint32_t count);
  ~OpenGLIndexBuffer() override;

  void Bind() const override;
  void Unbind() const override;

  std::uint32_t GetCount() const override { return count_; }
  std::uint32_t GetRendererId() const { return renderer_id_; }

 private:
  std::uint32_t renderer_id_ = 0;
  std::uint32_t count_ = 0;
};

}  // namespace engine::renderer

#endif  // SRC_ENGINE_RENDERER_OPENGL_BUFFER_H_
