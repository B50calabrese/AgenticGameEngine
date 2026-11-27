#ifndef ENGINE_UI_RECT_H_
#define ENGINE_UI_RECT_H_

#include "engine/math/vector2.h"

namespace engine::ui {

/**
 * @brief Axis-aligned 2D rectangle in screen/UI space (bottom-left origin).
 */
struct Rect {
  float x = 0.0f;
  float y = 0.0f;
  float width = 0.0f;
  float height = 0.0f;

  bool Contains(const engine::math::Vector2& point) const {
    return point.x >= x && point.x <= (x + width) &&
           point.y >= y && point.y <= (y + height);
  }

  bool Overlaps(const Rect& other) const {
    return x < (other.x + other.width) && (x + width) > other.x &&
           y < (other.y + other.height) && (y + height) > other.y;
  }
};

}  // namespace engine::ui

#endif  // ENGINE_UI_RECT_H_
