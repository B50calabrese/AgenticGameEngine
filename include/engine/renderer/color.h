#ifndef ENGINE_RENDERER_COLOR_H_
#define ENGINE_RENDERER_COLOR_H_

#include <algorithm>
#include <cstdint>

namespace engine::renderer {

/**
 * @brief Representation of an RGBA color with normalized float components [0.0, 1.0].
 */
struct Color {
  float r = 1.0f;
  float g = 1.0f;
  float b = 1.0f;
  float a = 1.0f;

  constexpr Color() = default;

  constexpr Color(float red, float green, float blue, float alpha = 1.0f)
      : r(red), g(green), b(blue), a(alpha) {}

  /**
   * @brief Creates a Color from 8-bit integer RGBA components [0, 255].
   */
  static constexpr Color FromRgba8(std::uint8_t red, std::uint8_t green,
                                  std::uint8_t blue, std::uint8_t alpha = 255) {
    return Color(static_cast<float>(red) / 255.0f,
                 static_cast<float>(green) / 255.0f,
                 static_cast<float>(blue) / 255.0f,
                 static_cast<float>(alpha) / 255.0f);
  }

  /**
   * @brief Creates a Color from a 32-bit hex integer (e.g. 0xFF5733FF).
   */
  static constexpr Color FromHex(std::uint32_t hex) {
    return FromRgba8(static_cast<std::uint8_t>((hex >> 24) & 0xFF),
                     static_cast<std::uint8_t>((hex >> 16) & 0xFF),
                     static_cast<std::uint8_t>((hex >> 8) & 0xFF),
                     static_cast<std::uint8_t>(hex & 0xFF));
  }

  bool operator==(const Color& other) const {
    return r == other.r && g == other.g && b == other.b && a == other.a;
  }

  bool operator!=(const Color& other) const {
    return !(*this == other);
  }

  Color operator+(const Color& other) const {
    return Color(std::clamp(r + other.r, 0.0f, 1.0f),
                 std::clamp(g + other.g, 0.0f, 1.0f),
                 std::clamp(b + other.b, 0.0f, 1.0f),
                 std::clamp(a + other.a, 0.0f, 1.0f));
  }

  Color operator*(float scalar) const {
    return Color(std::clamp(r * scalar, 0.0f, 1.0f),
                 std::clamp(g * scalar, 0.0f, 1.0f),
                 std::clamp(b * scalar, 0.0f, 1.0f),
                 std::clamp(a * scalar, 0.0f, 1.0f));
  }

  // Predefined standard colors
  static constexpr Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
  static constexpr Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
  static constexpr Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
  static constexpr Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
  static constexpr Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
  static constexpr Color Yellow() { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
  static constexpr Color Cyan() { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
  static constexpr Color Magenta() { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
  static constexpr Color Orange() { return Color(1.0f, 0.5f, 0.0f, 1.0f); }
  static constexpr Color Gray() { return Color(0.5f, 0.5f, 0.5f, 1.0f); }
  static constexpr Color DarkGray() { return Color(0.2f, 0.2f, 0.2f, 1.0f); }
  static constexpr Color LightGray() { return Color(0.8f, 0.8f, 0.8f, 1.0f); }
  static constexpr Color Clear() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_COLOR_H_
