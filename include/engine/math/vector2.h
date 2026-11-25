#ifndef ENGINE_MATH_VECTOR2_H_
#define ENGINE_MATH_VECTOR2_H_

#include <cmath>
#include <cstddef>
#include <string>

#include "engine/math/math_utils.h"

namespace engine::math {

/**
 * @brief 2-component floating-point vector for 2D positions, sizes, velocities, and directions.
 */
struct Vector2 {
  float x = 0.0f;
  float y = 0.0f;

  constexpr Vector2() = default;
  constexpr Vector2(float x_val, float y_val) : x(x_val), y(y_val) {}
  explicit constexpr Vector2(float scalar) : x(scalar), y(scalar) {}

  float& operator[](std::size_t index) {
    return (index == 0) ? x : y;
  }

  const float& operator[](std::size_t index) const {
    return (index == 0) ? x : y;
  }

  // Unary operators
  Vector2 operator-() const {
    return Vector2(-x, -y);
  }

  // Vector arithmetic
  Vector2 operator+(const Vector2& other) const {
    return Vector2(x + other.x, y + other.y);
  }

  Vector2 operator-(const Vector2& other) const {
    return Vector2(x - other.x, y - other.y);
  }

  Vector2 operator*(const Vector2& other) const {
    return Vector2(x * other.x, y * other.y);
  }

  Vector2 operator/(const Vector2& other) const {
    return Vector2(x / other.x, y / other.y);
  }

  // Scalar arithmetic
  Vector2 operator*(float scalar) const {
    return Vector2(x * scalar, y * scalar);
  }

  Vector2 operator/(float scalar) const {
    float inv = 1.0f / scalar;
    return Vector2(x * inv, y * inv);
  }

  friend Vector2 operator*(float scalar, const Vector2& vec) {
    return vec * scalar;
  }

  // Compound assignment
  Vector2& operator+=(const Vector2& other) {
    x += other.x;
    y += other.y;
    return *this;
  }

  Vector2& operator-=(const Vector2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }

  Vector2& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }

  Vector2& operator/=(float scalar) {
    float inv = 1.0f / scalar;
    x *= inv;
    y *= inv;
    return *this;
  }

  // Comparison
  bool operator==(const Vector2& other) const {
    return Approximately(x, other.x) && Approximately(y, other.y);
  }

  bool operator!=(const Vector2& other) const {
    return !(*this == other);
  }

  // Geometric calculations
  float LengthSquared() const {
    return (x * x) + (y * y);
  }

  float Length() const {
    return std::sqrt(LengthSquared());
  }

  Vector2 Normalized() const {
    float len = Length();
    if (len > kEpsilon) {
      return *this / len;
    }
    return Vector2(0.0f, 0.0f);
  }

  Vector2& Normalize() {
    float len = Length();
    if (len > kEpsilon) {
      *this /= len;
    } else {
      x = 0.0f;
      y = 0.0f;
    }
    return *this;
  }

  float Dot(const Vector2& other) const {
    return (x * other.x) + (y * other.y);
  }

  static float Dot(const Vector2& a, const Vector2& b) {
    return a.Dot(b);
  }

  /**
   * @brief 2D scalar cross product: returns a.x * b.y - a.y * b.x.
   */
  float Cross(const Vector2& other) const {
    return (x * other.y) - (y * other.x);
  }

  static float Cross(const Vector2& a, const Vector2& b) {
    return a.Cross(b);
  }

  float DistanceSquared(const Vector2& other) const {
    return (*this - other).LengthSquared();
  }

  float Distance(const Vector2& other) const {
    return (*this - other).Length();
  }

  /**
   * @brief Returns a counter-clockwise perpendicular vector: (-y, x).
   */
  Vector2 Perpendicular() const {
    return Vector2(-y, x);
  }

  /**
   * @brief Returns the angle of the vector in radians [ -pi, pi ].
   */
  float Angle() const {
    return std::atan2(y, x);
  }

  static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) {
    return Vector2(engine::math::Lerp(a.x, b.x, t),
                   engine::math::Lerp(a.y, b.y, t));
  }

  std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
  }

  // Standard presets
  static constexpr Vector2 Zero() { return Vector2(0.0f, 0.0f); }
  static constexpr Vector2 One() { return Vector2(1.0f, 1.0f); }
  static constexpr Vector2 UnitX() { return Vector2(1.0f, 0.0f); }
  static constexpr Vector2 UnitY() { return Vector2(0.0f, 1.0f); }
  static constexpr Vector2 Up() { return Vector2(0.0f, 1.0f); }
  static constexpr Vector2 Down() { return Vector2(0.0f, -1.0f); }
  static constexpr Vector2 Left() { return Vector2(-1.0f, 0.0f); }
  static constexpr Vector2 Right() { return Vector2(1.0f, 0.0f); }
};

}  // namespace engine::math

#endif  // ENGINE_MATH_VECTOR2_H_
