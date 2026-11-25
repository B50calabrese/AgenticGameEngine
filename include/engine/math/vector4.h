#ifndef ENGINE_MATH_VECTOR4_H_
#define ENGINE_MATH_VECTOR4_H_

#include <cmath>
#include <cstddef>
#include <string>

#include "engine/math/math_utils.h"
#include "engine/math/vector2.h"
#include "engine/math/vector3.h"

namespace engine::math {

/**
 * @brief 4-component floating-point vector for homogeneous coordinates and clip-space math.
 */
struct Vector4 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;
  float w = 0.0f;

  constexpr Vector4() = default;
  constexpr Vector4(float x_val, float y_val, float z_val, float w_val)
      : x(x_val), y(y_val), z(z_val), w(w_val) {}
  explicit constexpr Vector4(float scalar)
      : x(scalar), y(scalar), z(scalar), w(scalar) {}
  constexpr Vector4(const Vector3& v3, float w_val = 1.0f)
      : x(v3.x), y(v3.y), z(v3.z), w(w_val) {}
  constexpr Vector4(const Vector2& v2, float z_val = 0.0f, float w_val = 1.0f)
      : x(v2.x), y(v2.y), z(z_val), w(w_val) {}

  float& operator[](std::size_t index) {
    return (index == 0) ? x : ((index == 1) ? y : ((index == 2) ? z : w));
  }

  const float& operator[](std::size_t index) const {
    return (index == 0) ? x : ((index == 1) ? y : ((index == 2) ? z : w));
  }

  Vector2 ToVector2() const { return Vector2(x, y); }
  Vector3 ToVector3() const { return Vector3(x, y, z); }

  Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

  Vector4 operator+(const Vector4& other) const {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  Vector4 operator-(const Vector4& other) const {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  Vector4 operator*(float scalar) const {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
  }

  Vector4 operator/(float scalar) const {
    float inv = 1.0f / scalar;
    return Vector4(x * inv, y * inv, z * inv, w * inv);
  }

  float Dot(const Vector4& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
  }

  bool operator==(const Vector4& other) const {
    return Approximately(x, other.x) && Approximately(y, other.y) &&
           Approximately(z, other.z) && Approximately(w, other.w);
  }

  bool operator!=(const Vector4& other) const {
    return !(*this == other);
  }

  static constexpr Vector4 Zero() { return Vector4(0.0f, 0.0f, 0.0f, 0.0f); }
  static constexpr Vector4 One() { return Vector4(1.0f, 1.0f, 1.0f, 1.0f); }
};

}  // namespace engine::math

#endif  // ENGINE_MATH_VECTOR4_H_
