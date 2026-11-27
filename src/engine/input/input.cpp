#include "engine/input/input.h"

#include "engine/platform/window.h"

#if defined(ENGINE_ENABLE_GLFW)
#include <GLFW/glfw3.h>
#endif

namespace engine::input {

static engine::platform::Window* s_target_window = nullptr;

void Input::SetTargetWindow(engine::platform::Window* window) {
  s_target_window = window;
}

engine::platform::Window* Input::GetTargetWindow() {
  return s_target_window;
}

bool Input::IsKeyPressed(KeyCode key) {
#if defined(ENGINE_ENABLE_GLFW)
  if (!s_target_window) {
    return false;
  }
  auto* glfw_window = static_cast<GLFWwindow*>(s_target_window->GetNativeWindow());
  if (!glfw_window) {
    return false;
  }
  int state = glfwGetKey(glfw_window, static_cast<int>(key));
  return state == GLFW_PRESS || state == GLFW_REPEAT;
#else
  (void)key;
  return false;
#endif
}

bool Input::IsMouseButtonPressed(MouseButton button) {
#if defined(ENGINE_ENABLE_GLFW)
  if (!s_target_window) {
    return false;
  }
  auto* glfw_window = static_cast<GLFWwindow*>(s_target_window->GetNativeWindow());
  if (!glfw_window) {
    return false;
  }
  int state = glfwGetMouseButton(glfw_window, static_cast<int>(button));
  return state == GLFW_PRESS;
#else
  (void)button;
  return false;
#endif
}

engine::math::Vector2 Input::GetMousePosition() {
#if defined(ENGINE_ENABLE_GLFW)
  if (!s_target_window) {
    return engine::math::Vector2(0.0f, 0.0f);
  }
  auto* glfw_window = static_cast<GLFWwindow*>(s_target_window->GetNativeWindow());
  if (!glfw_window) {
    return engine::math::Vector2(0.0f, 0.0f);
  }
  double xpos = 0.0;
  double ypos = 0.0;
  glfwGetCursorPos(glfw_window, &xpos, &ypos);
  return engine::math::Vector2(static_cast<float>(xpos), static_cast<float>(ypos));
#else
  return engine::math::Vector2(0.0f, 0.0f);
#endif
}

float Input::GetMouseX() {
  return GetMousePosition().x;
}

float Input::GetMouseY() {
  return GetMousePosition().y;
}

}  // namespace engine::input
