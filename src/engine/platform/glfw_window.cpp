#include "engine/platform/glfw_window.h"

#if defined(ENGINE_ENABLE_GLFW)
#include <GLFW/glfw3.h>
#endif

#include "engine/core/assert.h"
#include "engine/core/log.h"

namespace engine::platform {

#if defined(ENGINE_ENABLE_GLFW)

static void GlfwErrorCallback(int error_code, const char* description) {
  ENGINE_LOG_ERROR("GLFW Error (" + std::to_string(error_code) + "): " + std::string(description));
}

GlfwWindow::GlfwWindow(const WindowProps& props) {
  Initialize(props);
}

GlfwWindow::~GlfwWindow() {
  Shutdown();
}

void GlfwWindow::Initialize(const WindowProps& props) {
  title_ = props.title;
  width_ = props.width;
  height_ = props.height;
  is_vsync_ = props.vsync;

  ENGINE_LOG_INFO("Creating window: " + title_ + " (" +
                  std::to_string(width_) + "x" + std::to_string(height_) + ")");

  if (s_glfw_window_count_ == 0) {
    int success = glfwInit();
    ENGINE_ASSERT(success, "Failed to initialize GLFW!");
    glfwSetErrorCallback(GlfwErrorCallback);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window_ = glfwCreateWindow(static_cast<int>(width_), static_cast<int>(height_),
                             title_.c_str(), nullptr, nullptr);

  if (!window_) {
    ENGINE_LOG_ERROR("Failed to create GLFW window: " + title_);
    return;
  }

  ++s_glfw_window_count_;

  glfwMakeContextCurrent(window_);
  SetVSync(is_vsync_);

  ENGINE_LOG_INFO("Window initialized successfully.");
}

void GlfwWindow::Shutdown() {
  if (window_) {
    glfwDestroyWindow(window_);
    window_ = nullptr;
    --s_glfw_window_count_;

    if (s_glfw_window_count_ == 0) {
      glfwTerminate();
      ENGINE_LOG_INFO("GLFW terminated cleanly.");
    }
  }
}

void GlfwWindow::OnUpdate() {
  if (!window_) {
    return;
  }
  glfwPollEvents();
  glfwSwapBuffers(window_);
}

bool GlfwWindow::ShouldClose() const {
  if (!window_) {
    return true;
  }
  return glfwWindowShouldClose(window_) != 0;
}

void GlfwWindow::SetVSync(bool enabled) {
  is_vsync_ = enabled;
  glfwSwapInterval(enabled ? 1 : 0);
}

#else

GlfwWindow::GlfwWindow(const WindowProps& props) {
  ENGINE_LOG_ERROR("GlfwWindow constructed but GLFW is disabled or not installed!");
  (void)props;
}

GlfwWindow::~GlfwWindow() = default;
void GlfwWindow::OnUpdate() {}
bool GlfwWindow::ShouldClose() const { return true; }
void GlfwWindow::SetVSync(bool enabled) { (void)enabled; }

#endif  // defined(ENGINE_ENABLE_GLFW)

std::unique_ptr<Window> Window::Create(const WindowProps& props) {
#if defined(ENGINE_ENABLE_GLFW)
  auto window = std::make_unique<GlfwWindow>(props);
  if (!window->IsInitialized()) {
    return nullptr;
  }
  return window;
#else
  (void)props;
  ENGINE_LOG_WARN("Window::Create requested, but engine was built without GLFW support.");
  ENGINE_LOG_WARN("Install 'libglfw3-dev libgl-dev' to enable graphical windowing.");
  return nullptr;
#endif
}

}  // namespace engine::platform
