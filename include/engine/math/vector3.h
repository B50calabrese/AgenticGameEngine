#ifndef ENGINE_MATH_VECTOR3_H_
#define ENGINE_MATH_VECTOR3_H_

#include <cmath>
#include <cstddef>
#include <string>

#include "engine/math/math_utils.h"
#include "engine/math/vector2.h"

namespace engine::math {

/**
 * @brief 3-component floating-point vector for 3D/2.5D coordinates, directions, scales, and normals.
 */
struct Vector3 {
  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  constexpr Vector3() = default;
  constexpr Vector3(float x_val, float y_val, float z_val)
      : x(x_val), y(y_val), z(z_val) {}
  explicit constexpr Vector3(float scalar)
      : x(scalar), y(scalar), z(scalar) {}
  constexpr Vector3(const Vector2& v2, float z_val = 0.0f)
      : x(v2.x), y(v2.y), z(z_val) {}

  float& operator[](std::size_t index) {
    return (index == 0) ? x : ((index == 1) ? y : z);
  }

  const float& operator[](std::size_t index) const {
    return (index == 0) ? x : ((index == 1) ? y : z);
  }

  Vector2 ToVector2() const {
    return Vector2(x, y);
  }

  // Unary operators
  Vector3 operator-() const {
    return Vector3(-x, -y, -z);
  }

  // Vector arithmetic
  Vector3 operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  Vector3 operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  Vector3 operator*(const Vector3& other) const {
    return Vector3(x * other.x, y * other.y, z * other.z);
  }

  Vector3 operator/(const Vector3& other) const {
    return Vector3(x / other.x, y / other.y, z / other.z);
  }

  // Scalar arithmetic
  Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  Vector3 operator/(float scalar) const {
    float inv = 1.0f / scalar;
    return Vector3(x * inv, y * inv, z * inv);
  }

  friend Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
  }

  // Compound assignment
  Vector3& operator+=(const Vector3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vector3& operator-=(const Vector3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }

  Vector3& operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
  }

  Vector3& operator/=(float scalar) {
    float inv = 1.0f / scalar;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }

  // Comparison
  bool operator==(const Vector3& other) const {
    return Approximately(x, other.x) && Approximately(y, other.y) &&
           Approximately(z, other.z);
  }

  bool operator!=(const Vector3& other) const {
    return !(*this == other);
  }

  // Geometric calculations
  float LengthSquared() const {
    return (x * x) + (y * y) + (z * z);
  }

  float Length() const {
    return std::sqrt(LengthSquared());
  }

  Vector3 Normalized() const {
    float len = Length();
    if (len > kEpsilon) {
      return *this / len;
    }
    return Vector3(0.0f, 0.0f, 0.0f);
  }

  Vector3& Normalize() {
    float len = Length();
    if (len > kEpsilon) {
      *this /= len;
    } else {
      x = 0.0f;
      y = 0.0f;
      z = 0.0f;
    }
    return *this;
  }

  float Dot(const Vector3& other) const {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }

  static float Dot(const Vector3& a, const Vector3& b) {
    return a.Dot(b);
  }

  Vector3 Cross(const Vector3& other) const {
    return Vector3((y * other.z) - (z * other.y),
                   (z * other.x) - (x * other.z),
                   (x * other.y) - (y * other.x));
  }

  static Vector3 Cross(const Vector3& a, const Vector3& b) {
    return a.Cross(b);
  }

  float DistanceSquared(const Vector3& other) const {
    return (*this - other).LengthSquared();
  }

  float Distance(const Vector3& other) const {
    return (*this - other).Length();
  }

  static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) {
    return Vector3(engine::math::Lerp(a.x, b.x, t),
                   engine::math::Lerp(a.y, b.y, t),
                   engine::math::Lerp(a.z, b.z, t));
  }

  std::string ToString() const {
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " +
           std::to_string(z) + ")";
  }

  // Standard presets
  static constexpr Vector3 Zero() { return Vector3(0.0f, 0.0f, 0.0f); }
  static constexpr Vector3 One() { return Vector3(1.0f, 1.0f, 1.0f); }
  static constexpr Vector3 UnitX() { return Vector3(1.0f, 0.0f, 0.0f); }
  static constexpr Vector3 UnitY() { return Vector3(0.0f, 1.0f, 0.0f); }
  static constexpr Vector3 UnitZ() { return Vector3(0.0f, 0.0f, 1.0f); }
  static constexpr Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
  static constexpr Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
  static constexpr Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
  static constexpr Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
  static constexpr Vector3 Forward() { return Vector3(0.0f, 0.0f, -1.0f); }
  static constexpr Vector3 Back() { return Vector3(0.0f, 0.0f, 1.0f); }
};

}  // namespace engine::math

#endif  // ENGINE_MATH_VECTOR3_H_
