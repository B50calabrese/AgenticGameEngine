#ifndef ENGINE_PLATFORM_GLFW_WINDOW_H_
#define ENGINE_PLATFORM_GLFW_WINDOW_H_

#include <memory>

#include "engine/platform/window.h"

struct GLFWwindow;

namespace engine::platform {

class GlfwWindow : public Window {
 public:
  explicit GlfwWindow(const WindowProps& props);
  ~GlfwWindow() override;

  void OnUpdate() override;

  std::uint32_t GetWidth() const override { return width_; }
  std::uint32_t GetHeight() const override { return height_; }

  bool ShouldClose() const override;
  void SetVSync(bool enabled) override;
  bool IsVSync() const override { return is_vsync_; }

  void* GetNativeWindow() const override {
    return reinterpret_cast<void*>(window_);
  }

  bool IsInitialized() const { return window_ != nullptr; }

 private:
  void Initialize(const WindowProps& props);
  void Shutdown();

  GLFWwindow* window_ = nullptr;
  std::string title_;
  std::uint32_t width_ = 0;
  std::uint32_t height_ = 0;
  bool is_vsync_ = true;

  static inline int s_glfw_window_count_ = 0;
};

}  // namespace engine::platform

#endif  // ENGINE_PLATFORM_GLFW_WINDOW_H_
