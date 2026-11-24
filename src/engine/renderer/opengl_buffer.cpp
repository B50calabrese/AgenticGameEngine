#include "engine/renderer/opengl_buffer.h"

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
#include <glad/glad.h>
#endif

#include "engine/core/assert.h"
#include "engine/core/log.h"

namespace engine::renderer {

std::uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
      return 4;
    case ShaderDataType::Float2:
      return 4 * 2;
    case ShaderDataType::Float3:
      return 4 * 3;
    case ShaderDataType::Float4:
      return 4 * 4;
    case ShaderDataType::Mat3:
      return 4 * 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4 * 4;
    case ShaderDataType::Int:
      return 4;
    case ShaderDataType::Int2:
      return 4 * 2;
    case ShaderDataType::Int3:
      return 4 * 3;
    case ShaderDataType::Int4:
      return 4 * 4;
    case ShaderDataType::Bool:
      return 1;
    case ShaderDataType::None:
      return 0;
  }
  return 0;
}

std::uint32_t BufferElement::GetComponentCount() const {
  switch (type) {
    case ShaderDataType::Float:
      return 1;
    case ShaderDataType::Float2:
      return 2;
    case ShaderDataType::Float3:
      return 3;
    case ShaderDataType::Float4:
      return 4;
    case ShaderDataType::Mat3:
      return 3 * 3;
    case ShaderDataType::Mat4:
      return 4 * 4;
    case ShaderDataType::Int:
      return 1;
    case ShaderDataType::Int2:
      return 2;
    case ShaderDataType::Int3:
      return 3;
    case ShaderDataType::Int4:
      return 4;
    case ShaderDataType::Bool:
      return 1;
    case ShaderDataType::None:
      return 0;
  }
  return 0;
}

// =============================================================================
// VertexBuffer Implementations
// =============================================================================

std::shared_ptr<VertexBuffer> VertexBuffer::Create(std::uint32_t size) {
  return std::make_shared<OpenGLVertexBuffer>(size);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(const void* vertices,
                                                   std::uint32_t size) {
  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(std::uint32_t size) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glGenBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
#else
  (void)size;
#endif
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, std::uint32_t size) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glGenBuffers(1, &renderer_id_);
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
#else
  (void)vertices;
  (void)size;
#endif
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glDeleteBuffers(1, &renderer_id_);
  }
#endif
}

void OpenGLVertexBuffer::Bind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
#endif
}

void OpenGLVertexBuffer::Unbind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void OpenGLVertexBuffer::SetData(const void* data, std::uint32_t size) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindBuffer(GL_ARRAY_BUFFER, renderer_id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
#else
  (void)data;
  (void)size;
#endif
}

// =============================================================================
// IndexBuffer Implementations
// =============================================================================

std::shared_ptr<IndexBuffer> IndexBuffer::Create(const std::uint32_t* indices,
                                                 std::uint32_t count) {
  return std::make_shared<OpenGLIndexBuffer>(indices, count);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::uint32_t* indices,
                                     std::uint32_t count)
    : count_(count) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glGenBuffers(1, &renderer_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), indices,
               GL_STATIC_DRAW);
#else
  (void)indices;
#endif
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glDeleteBuffers(1, &renderer_id_);
  }
#endif
}

void OpenGLIndexBuffer::Bind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_);
#endif
}

void OpenGLIndexBuffer::Unbind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#endif
}

}  // namespace engine::renderer
