#ifndef ENGINE_MATH_MATRIX4_H_
#define ENGINE_MATH_MATRIX4_H_

#include <array>
#include <cstddef>
#include <string>

#include "engine/math/vector3.h"
#include "engine/math/vector4.h"

namespace engine::math {

/**
 * @brief 4x4 column-major floating-point matrix for 2D/3D transformations and projections.
 *
 * Compatible with OpenGL shaders (elements array can be passed directly to glUniformMatrix4fv).
 */
class Matrix4 {
 public:
  /**
   * @brief Elements stored in column-major order:
   * [ 0  4  8 12 ]
   * [ 1  5  9 13 ]
   * [ 2  6 10 14 ]
   * [ 3  7 11 15 ]
   */
  std::array<float, 16> elements = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  Matrix4();
  explicit Matrix4(float diagonal);
  explicit Matrix4(const float elements_array[16]);
  Matrix4(const std::array<float, 16>& elements_array);

  float& operator()(std::size_t row, std::size_t col) {
    return elements[col * 4 + row];
  }

  const float& operator()(std::size_t row, std::size_t col) const {
    return elements[col * 4 + row];
  }

  float& operator[](std::size_t index) {
    return elements[index];
  }

  const float& operator[](std::size_t index) const {
    return elements[index];
  }

  const float* Data() const {
    return elements.data();
  }

  float* Data() {
    return elements.data();
  }

  // Matrix multiplication
  Matrix4 operator*(const Matrix4& other) const;
  Matrix4& operator*=(const Matrix4& other);

  // Vector transformations
  Vector4 operator*(const Vector4& vec) const;
  Vector3 TransformPoint(const Vector3& point) const;
  Vector3 TransformDirection(const Vector3& direction) const;

  bool operator==(const Matrix4& other) const;
  bool operator!=(const Matrix4& other) const;

  // Algebraic methods
  Matrix4 Transposed() const;
  float Determinant() const;
  Matrix4 Inverted() const;

  // Static Factory Methods
  static Matrix4 Identity();
  static Matrix4 Translation(const Vector3& translation);
  static Matrix4 Translation(float x, float y, float z = 0.0f);
  static Matrix4 RotationX(float radians);
  static Matrix4 RotationY(float radians);
  static Matrix4 RotationZ(float radians);
  static Matrix4 Rotation(float radians, const Vector3& axis);
  static Matrix4 Scale(const Vector3& scale);
  static Matrix4 Scale(float x, float y, float z = 1.0f);

  /**
   * @brief Creates an orthographic projection matrix.
   */
  static Matrix4 Orthographic(float left, float right, float bottom, float top,
                              float near_clip = -1.0f, float far_clip = 1.0f);

  /**
   * @brief Creates a perspective projection matrix.
   */
  static Matrix4 Perspective(float fov_radians, float aspect_ratio,
                             float near_clip, float far_clip);

  /**
   * @brief Creates a view matrix looking from eye towards target.
   */
  static Matrix4 LookAt(const Vector3& eye, const Vector3& target,
                        const Vector3& up);

  std::string ToString() const;
};

}  // namespace engine::math

#endif  // ENGINE_MATH_MATRIX4_H_
