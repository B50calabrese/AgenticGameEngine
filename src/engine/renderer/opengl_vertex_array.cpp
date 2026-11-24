#include "engine/renderer/opengl_vertex_array.h"

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
#include <glad/glad.h>
#endif

#include "engine/core/assert.h"

namespace engine::renderer {

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    case ShaderDataType::None:
      return GL_NONE;
  }
  return GL_NONE;
}
#endif

std::shared_ptr<VertexArray> VertexArray::Create() {
  return std::make_shared<OpenGLVertexArray>();
}

OpenGLVertexArray::OpenGLVertexArray() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glGenVertexArrays(1, &renderer_id_);
#endif
}

OpenGLVertexArray::~OpenGLVertexArray() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glDeleteVertexArrays(1, &renderer_id_);
  }
#endif
}

void OpenGLVertexArray::Bind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindVertexArray(renderer_id_);
#endif
}

void OpenGLVertexArray::Unbind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindVertexArray(0);
#endif
}

void OpenGLVertexArray::AddVertexBuffer(
    const std::shared_ptr<VertexBuffer>& vertex_buffer) {
  ENGINE_ASSERT(!vertex_buffer->GetLayout().GetElements().empty(),
                "Vertex Buffer has no layout specified!");

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindVertexArray(renderer_id_);
  vertex_buffer->Bind();

  const auto& layout = vertex_buffer->GetLayout();
  for (const auto& element : layout) {
    glEnableVertexAttribArray(vertex_buffer_index_);
    glVertexAttribPointer(
        vertex_buffer_index_,
        static_cast<GLint>(element.GetComponentCount()),
        ShaderDataTypeToOpenGLBaseType(element.type),
        element.normalized ? GL_TRUE : GL_FALSE,
        static_cast<GLsizei>(layout.GetStride()),
        reinterpret_cast<const void*>(element.offset));
    ++vertex_buffer_index_;
  }
#endif

  vertex_buffers_.push_back(vertex_buffer);
}

void OpenGLVertexArray::SetIndexBuffer(
    const std::shared_ptr<IndexBuffer>& index_buffer) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glBindVertexArray(renderer_id_);
  index_buffer->Bind();
#endif

  index_buffer_ = index_buffer;
}

}  // namespace engine::renderer
