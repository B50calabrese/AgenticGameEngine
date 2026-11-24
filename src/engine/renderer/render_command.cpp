#include "engine/renderer/render_command.h"

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#include "engine/core/log.h"

namespace engine::renderer {

static bool s_initialized = false;

bool RenderCommand::Initialize() {
  if (s_initialized) {
    return true;
  }

#if defined(ENGINE_ENABLE_GLFW)
  int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
  if (!status) {
    ENGINE_LOG_ERROR("Failed to initialize GLAD OpenGL loader!");
    return false;
  }
  s_initialized = true;

  auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
  auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
  auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));

  ENGINE_LOG_INFO("OpenGL Context Initialized:");
  ENGINE_LOG_INFO("  Version:  " + std::string(version ? version : "Unknown"));
  ENGINE_LOG_INFO("  Vendor:   " + std::string(vendor ? vendor : "Unknown"));
  ENGINE_LOG_INFO("  Renderer: " + std::string(renderer ? renderer : "Unknown"));

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  return true;
#else
  ENGINE_LOG_WARN("RenderCommand::Initialize: Compiled without OpenGL/GLFW support.");
  return false;
#endif
}

bool RenderCommand::IsInitialized() {
  return s_initialized;
}

void RenderCommand::SetViewport(std::uint32_t x, std::uint32_t y,
                                std::uint32_t width, std::uint32_t height) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
             static_cast<GLsizei>(width), static_cast<GLsizei>(height));
#else
  (void)x;
  (void)y;
  (void)width;
  (void)height;
#endif
}

void RenderCommand::SetClearColor(const Color& color) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glClearColor(color.r, color.g, color.b, color.a);
#else
  (void)color;
#endif
}

void RenderCommand::Clear() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif
}

void RenderCommand::DrawIndexed(
    const std::shared_ptr<VertexArray>& vertex_array,
    std::uint32_t index_count) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  vertex_array->Bind();
  std::uint32_t count = index_count != 0
                            ? index_count
                            : vertex_array->GetIndexBuffer()->GetCount();
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count), GL_UNSIGNED_INT,
                 nullptr);
#else
  (void)vertex_array;
  (void)index_count;
#endif
}

void RenderCommand::DrawLines(
    const std::shared_ptr<VertexArray>& vertex_array,
    std::uint32_t vertex_count) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  vertex_array->Bind();
  glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(vertex_count));
#else
  (void)vertex_array;
  (void)vertex_count;
#endif
}

void RenderCommand::SetLineWidth(float width) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glLineWidth(width);
#else
  (void)width;
#endif
}

}  // namespace engine::renderer
