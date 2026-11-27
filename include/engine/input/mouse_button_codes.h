#ifndef ENGINE_INPUT_MOUSE_BUTTON_CODES_H_
#define ENGINE_INPUT_MOUSE_BUTTON_CODES_H_

#include <cstdint>

namespace engine::input {

/**
 * @brief Standard mouse button codes. Aligned with standard GLFW mappings.
 */
enum class MouseButton : std::uint8_t {
  Button0 = 0,
  Button1 = 1,
  Button2 = 2,
  Button3 = 3,
  Button4 = 4,
  Button5 = 5,
  Button6 = 6,
  Button7 = 7,

  Left = Button0,
  Right = Button1,
  Middle = Button2,
  Last = Button7
};

}  // namespace engine::input

#endif  // ENGINE_INPUT_MOUSE_BUTTON_CODES_H_
