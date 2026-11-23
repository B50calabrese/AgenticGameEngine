# Platform & Windowing Subsystem

The **Platform Subsystem** abstracts operating system facilities, display window creation, event polling, and graphics context management.

* **Status**: Active & Implemented
* **Public Headers**: `include/engine/platform/`
* **Internal Source**: `src/engine/platform/`
* **Tests**: `tests/window_test.cpp`
* **Demos**: `demos/window/main.cpp` (`make window-demo`)

---

## 1. Architectural Role & Responsibilities

In keeping with our consumer-first and cross-platform architecture:
* The core engine and downstream games interact strictly with the abstract `engine::platform::Window` interface.
* Low-level window handles (`GLFWwindow*`) and backend-specific calls (`glfwPollEvents()`, `glfwSwapBuffers()`) are completely encapsulated within `GlfwWindow` in `src/engine/platform/`.
* Allows seamless support for alternative windowing backends (e.g., SDL2, Win32, Wayland native, or Headless mock) without breaking downstream game logic.

---

## 2. Public API & Key Classes

### A. `WindowProps`
Configuration structure passed to `Window::Create`:
```cpp
struct WindowProps {
  std::string title = "Agentic Game Engine";
  std::uint32_t width = 1280;
  std::uint32_t height = 720;
  bool vsync = true;
};
```

### B. `Window` Interface
Abstract base class defining the window lifecycle contract:
```cpp
class Window {
 public:
  virtual ~Window() = default;

  virtual void OnUpdate() = 0;
  virtual std::uint32_t GetWidth() const = 0;
  virtual std::uint32_t GetHeight() const = 0;
  virtual bool ShouldClose() const = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;
  virtual void* GetNativeWindow() const = 0;

  static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};
```

---

## 3. Usage Example

```cpp
#include "engine/engine.h"
#include "engine/platform/window.h"

int main() {
  engine::core::Log::Initialize();

  engine::platform::WindowProps props;
  props.title = "My Game Window";
  props.width = 1280;
  props.height = 720;
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Failed to create platform window.");
    return 1;
  }

  // Main application loop
  while (!window->ShouldClose()) {
    // 1. Process OS events & swap buffers
    window->OnUpdate();
  }

  return 0;
}
```

---

## 4. Invariants & Lifecycle Rules

1. **RAII Enforcement**: The `GlfwWindow` constructor manages `glfwInit()` on first instantiation and `glfwTerminate()` when the final window instance is destroyed.
2. **Context Creation**: `GlfwWindow` requests an OpenGL 3.3 Core Profile context.
3. **No External Header Leakage**: Neither `GLFW/glfw3.h` nor any X11/Win32 header is ever included from `include/engine/platform/window.h`.

---

## 5. Related ADRs
* [ADR-0003: Abstract Window Interface & GLFW Backend Architecture](../adr/0003-abstract-window-interface-and-glfw-backend.md)
