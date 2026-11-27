#ifndef ENGINE_INPUT_INPUT_H_
#define ENGINE_INPUT_INPUT_H_

#include "engine/input/key_codes.h"
#include "engine/input/mouse_button_codes.h"
#include "engine/math/vector2.h"

namespace engine::platform {
class Window;
}  // namespace engine::platform

namespace engine::input {

/**
 * @brief Polling-based input interface for keyboard and mouse states.
 */
class Input {
 public:
  /**
   * @brief Associates the input system with an active platform window.
   */
  static void SetTargetWindow(engine::platform::Window* window);

  /**
   * @brief Retrieves the currently targeted window, if any.
   */
  static engine::platform::Window* GetTargetWindow();

  /**
   * @brief Checks if a specific keyboard key is currently held down.
   */
  static bool IsKeyPressed(KeyCode key);

  /**
   * @brief Checks if a specific mouse button is currently held down.
   */
  static bool IsMouseButtonPressed(MouseButton button);

  /**
   * @brief Returns current mouse cursor position in window coordinates (pixels).
   */
  static engine::math::Vector2 GetMousePosition();

  static float GetMouseX();
  static float GetMouseY();
};

}  // namespace engine::input

#endif  // ENGINE_INPUT_INPUT_H_
