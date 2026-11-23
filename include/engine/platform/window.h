#ifndef ENGINE_PLATFORM_WINDOW_H_
#define ENGINE_PLATFORM_WINDOW_H_

#include <cstdint>
#include <memory>
#include <string>

namespace engine::platform {

/**
 * @brief Configuration properties for window creation.
 */
struct WindowProps {
  std::string title = "Agentic Game Engine";
  std::uint32_t width = 1280;
  std::uint32_t height = 720;
  bool vsync = true;
};

/**
 * @brief Platform-agnostic interface for window lifecycle and presentation.
 *
 * This abstraction isolates the rest of the engine from specific windowing
 * toolkits (GLFW, SDL, Win32, etc.).
 */
class Window {
 public:
  virtual ~Window() = default;

  /**
   * @brief Processes pending OS events and swaps front/back buffers.
   */
  virtual void OnUpdate() = 0;

  /**
   * @brief Returns current window width in screen coordinates.
   */
  virtual std::uint32_t GetWidth() const = 0;

  /**
   * @brief Returns current window height in screen coordinates.
   */
  virtual std::uint32_t GetHeight() const = 0;

  /**
   * @brief Checks if a close event has been requested.
   */
  virtual bool ShouldClose() const = 0;

  /**
   * @brief Enables or disables Vertical Synchronization.
   */
  virtual void SetVSync(bool enabled) = 0;

  /**
   * @brief Returns current VSync status.
   */
  virtual bool IsVSync() const = 0;

  /**
   * @brief Returns the underlying native window handle (e.g., GLFWwindow*).
   */
  virtual void* GetNativeWindow() const = 0;

  /**
   * @brief Factory method creating the appropriate platform window instance.
   *
   * @param props Window configuration options.
   * @return A unique_ptr to the constructed Window, or nullptr on failure.
   */
  static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};

}  // namespace engine::platform

#endif  // ENGINE_PLATFORM_WINDOW_H_
