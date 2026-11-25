#include "engine/math/matrix4.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "engine/core/assert.h"
#include "engine/math/math_utils.h"

namespace engine::math {

Matrix4::Matrix4() {
  elements = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };
}

Matrix4::Matrix4(float diagonal) {
  elements = {
      diagonal, 0.0f,     0.0f,     0.0f,
      0.0f,     diagonal, 0.0f,     0.0f,
      0.0f,     0.0f,     diagonal, 0.0f,
      0.0f,     0.0f,     0.0f,     diagonal
  };
}

Matrix4::Matrix4(const float elements_array[16]) {
  for (std::size_t i = 0; i < 16; ++i) {
    elements[i] = elements_array[i];
  }
}

Matrix4::Matrix4(const std::array<float, 16>& elements_array)
    : elements(elements_array) {}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
  Matrix4 result(0.0f);
  for (std::size_t col = 0; col < 4; ++col) {
    for (std::size_t row = 0; row < 4; ++row) {
      float sum = 0.0f;
      for (std::size_t k = 0; k < 4; ++k) {
        sum += (*this)(row, k) * other(k, col);
      }
      result(row, col) = sum;
    }
  }
  return result;
}

Matrix4& Matrix4::operator*=(const Matrix4& other) {
  *this = *this * other;
  return *this;
}

Vector4 Matrix4::operator*(const Vector4& vec) const {
  return Vector4(
      (*this)(0, 0) * vec.x + (*this)(0, 1) * vec.y + (*this)(0, 2) * vec.z + (*this)(0, 3) * vec.w,
      (*this)(1, 0) * vec.x + (*this)(1, 1) * vec.y + (*this)(1, 2) * vec.z + (*this)(1, 3) * vec.w,
      (*this)(2, 0) * vec.x + (*this)(2, 1) * vec.y + (*this)(2, 2) * vec.z + (*this)(2, 3) * vec.w,
      (*this)(3, 0) * vec.x + (*this)(3, 1) * vec.y + (*this)(3, 2) * vec.z + (*this)(3, 3) * vec.w);
}

Vector3 Matrix4::TransformPoint(const Vector3& point) const {
  float w = (*this)(3, 0) * point.x + (*this)(3, 1) * point.y +
            (*this)(3, 2) * point.z + (*this)(3, 3);
  float inv_w = (std::abs(w) > kEpsilon) ? (1.0f / w) : 1.0f;

  return Vector3(
      ((*this)(0, 0) * point.x + (*this)(0, 1) * point.y + (*this)(0, 2) * point.z + (*this)(0, 3)) * inv_w,
      ((*this)(1, 0) * point.x + (*this)(1, 1) * point.y + (*this)(1, 2) * point.z + (*this)(1, 3)) * inv_w,
      ((*this)(2, 0) * point.x + (*this)(2, 1) * point.y + (*this)(2, 2) * point.z + (*this)(2, 3)) * inv_w);
}

Vector3 Matrix4::TransformDirection(const Vector3& direction) const {
  return Vector3(
      (*this)(0, 0) * direction.x + (*this)(0, 1) * direction.y + (*this)(0, 2) * direction.z,
      (*this)(1, 0) * direction.x + (*this)(1, 1) * direction.y + (*this)(1, 2) * direction.z,
      (*this)(2, 0) * direction.x + (*this)(2, 1) * direction.y + (*this)(2, 2) * direction.z);
}

bool Matrix4::operator==(const Matrix4& other) const {
  for (std::size_t i = 0; i < 16; ++i) {
    if (!Approximately(elements[i], other.elements[i])) {
      return false;
    }
  }
  return true;
}

bool Matrix4::operator!=(const Matrix4& other) const {
  return !(*this == other);
}

Matrix4 Matrix4::Transposed() const {
  Matrix4 result;
  for (std::size_t col = 0; col < 4; ++col) {
    for (std::size_t row = 0; row < 4; ++row) {
      result(row, col) = (*this)(col, row);
    }
  }
  return result;
}

float Matrix4::Determinant() const {
  const float* m = elements.data();

  return m[12] * m[9]  * m[6]  * m[3]   - m[8] * m[13] * m[6]  * m[3]   -
         m[12] * m[5]  * m[10] * m[3]   + m[4] * m[13] * m[10] * m[3]   +
         m[8]  * m[5]  * m[14] * m[3]   - m[4] * m[9]  * m[14] * m[3]   -
         m[12] * m[9]  * m[2]  * m[7]   + m[8] * m[13] * m[2]  * m[7]   +
         m[12] * m[1]  * m[10] * m[7]   - m[0] * m[13] * m[10] * m[7]   -
         m[8]  * m[1]  * m[14] * m[7]   + m[0] * m[9]  * m[14] * m[7]   +
         m[12] * m[5]  * m[2]  * m[11]  - m[4] * m[13] * m[2]  * m[11]  -
         m[12] * m[1]  * m[6]  * m[11]  + m[0] * m[13] * m[6]  * m[11]  +
         m[4]  * m[1]  * m[14] * m[11]  - m[0] * m[5]  * m[14] * m[11]  -
         m[8]  * m[5]  * m[2]  * m[15]  + m[4] * m[9]  * m[2]  * m[15]  +
         m[8]  * m[1]  * m[6]  * m[15]  - m[0] * m[9]  * m[6]  * m[15]  -
         m[4]  * m[1]  * m[10] * m[15]  + m[0] * m[5]  * m[10] * m[15];
}

Matrix4 Matrix4::Inverted() const {
  const float* m = elements.data();
  float inv[16];

  inv[0] = m[5]  * m[10] * m[15] - 
           m[5]  * m[11] * m[14] - 
           m[9]  * m[6]  * m[15] + 
           m[9]  * m[7]  * m[14] +
           m[13] * m[6]  * m[11] - 
           m[13] * m[7]  * m[10];

  inv[4] = -m[4]  * m[10] * m[15] + 
            m[4]  * m[11] * m[14] + 
            m[8]  * m[6]  * m[15] - 
            m[8]  * m[7]  * m[14] - 
            m[12] * m[6]  * m[11] + 
            m[12] * m[7]  * m[10];

  inv[8] = m[4]  * m[9] * m[15] - 
           m[4]  * m[11] * m[13] - 
           m[8]  * m[5] * m[15] + 
           m[8]  * m[7] * m[13] + 
           m[12] * m[5] * m[11] - 
           m[12] * m[7] * m[9];

  inv[12] = -m[4]  * m[9] * m[14] + 
             m[4]  * m[10] * m[13] +
             m[8]  * m[5] * m[14] - 
             m[8]  * m[6] * m[13] - 
             m[12] * m[5] * m[10] + 
             m[12] * m[6] * m[9];

  inv[1] = -m[1]  * m[10] * m[15] + 
            m[1]  * m[11] * m[14] + 
            m[9]  * m[2] * m[15] - 
            m[9]  * m[3] * m[14] - 
            m[13] * m[2] * m[11] + 
            m[13] * m[3] * m[10];

  inv[5] = m[0]  * m[10] * m[15] - 
           m[0]  * m[11] * m[14] - 
           m[8]  * m[2] * m[15] + 
           m[8]  * m[3] * m[14] + 
           m[12] * m[2] * m[11] - 
           m[12] * m[3] * m[10];

  inv[9] = -m[0]  * m[9] * m[15] + 
            m[0]  * m[11] * m[13] + 
            m[8]  * m[1] * m[15] - 
            m[8]  * m[3] * m[13] - 
            m[12] * m[1] * m[11] + 
            m[12] * m[3] * m[9];

  inv[13] = m[0]  * m[9] * m[14] - 
            m[0]  * m[10] * m[13] - 
            m[8]  * m[1] * m[14] + 
            m[8]  * m[2] * m[13] + 
            m[12] * m[1] * m[10] - 
            m[12] * m[2] * m[9];

  inv[2] = m[1]  * m[6] * m[15] - 
           m[1]  * m[7] * m[14] - 
           m[5]  * m[2] * m[15] + 
           m[5]  * m[3] * m[14] + 
           m[13] * m[2] * m[7] - 
           m[13] * m[3] * m[6];

  inv[6] = -m[0]  * m[6] * m[15] + 
            m[0]  * m[7] * m[14] + 
            m[4]  * m[2] * m[15] - 
            m[4]  * m[3] * m[14] - 
            m[12] * m[2] * m[7] + 
            m[12] * m[3] * m[6];

  inv[10] = m[0]  * m[5] * m[15] - 
            m[0]  * m[7] * m[13] - 
            m[4]  * m[1] * m[15] + 
            m[4]  * m[3] * m[13] + 
            m[12] * m[1] * m[7] - 
            m[12] * m[3] * m[5];

  inv[14] = -m[0]  * m[5] * m[14] + 
             m[0]  * m[6] * m[13] + 
             m[4]  * m[1] * m[14] - 
             m[4]  * m[2] * m[13] - 
             m[12] * m[1] * m[6] + 
             m[12] * m[2] * m[5];

  inv[3] = -m[1] * m[6] * m[11] + 
            m[1] * m[7] * m[10] + 
            m[5] * m[2] * m[11] - 
            m[5] * m[3] * m[10] - 
            m[9] * m[2] * m[7] + 
            m[9] * m[3] * m[6];

  inv[7] = m[0] * m[6] * m[11] - 
           m[0] * m[7] * m[10] - 
           m[4] * m[2] * m[11] + 
           m[4] * m[3] * m[10] + 
           m[8] * m[2] * m[7] - 
           m[8] * m[3] * m[6];

  inv[11] = -m[0] * m[5] * m[11] + 
             m[0] * m[7] * m[9] + 
             m[4] * m[1] * m[11] - 
             m[4] * m[3] * m[9] - 
             m[8] * m[1] * m[7] + 
             m[8] * m[3] * m[5];

  inv[15] = m[0] * m[5] * m[10] - 
            m[0] * m[6] * m[9] - 
            m[4] * m[1] * m[10] + 
            m[4] * m[2] * m[9] + 
            m[8] * m[1] * m[6] - 
            m[8] * m[2] * m[5];

  float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
  if (std::abs(det) < kEpsilon) {
    return Matrix4::Identity();
  }

  float inv_det = 1.0f / det;
  Matrix4 result;
  for (int i = 0; i < 16; ++i) {
    result.elements[i] = inv[i] * inv_det;
  }
  return result;
}

Matrix4 Matrix4::Identity() {
  return Matrix4();
}

Matrix4 Matrix4::Translation(const Vector3& translation) {
  Matrix4 result;
  result.elements[12] = translation.x;
  result.elements[13] = translation.y;
  result.elements[14] = translation.z;
  return result;
}

Matrix4 Matrix4::Translation(float x, float y, float z) {
  return Translation(Vector3(x, y, z));
}

Matrix4 Matrix4::RotationX(float radians) {
  Matrix4 result;
  float c = std::cos(radians);
  float s = std::sin(radians);

  result(1, 1) = c;
  result(1, 2) = -s;
  result(2, 1) = s;
  result(2, 2) = c;
  return result;
}

Matrix4 Matrix4::RotationY(float radians) {
  Matrix4 result;
  float c = std::cos(radians);
  float s = std::sin(radians);

  result(0, 0) = c;
  result(0, 2) = s;
  result(2, 0) = -s;
  result(2, 2) = c;
  return result;
}

Matrix4 Matrix4::RotationZ(float radians) {
  Matrix4 result;
  float c = std::cos(radians);
  float s = std::sin(radians);

  result(0, 0) = c;
  result(0, 1) = -s;
  result(1, 0) = s;
  result(1, 1) = c;
  return result;
}

Matrix4 Matrix4::Rotation(float radians, const Vector3& axis) {
  Matrix4 result;
  Vector3 norm = axis.Normalized();
  float c = std::cos(radians);
  float s = std::sin(radians);
  float omc = 1.0f - c;

  result(0, 0) = norm.x * norm.x * omc + c;
  result(1, 0) = norm.y * norm.x * omc + norm.z * s;
  result(2, 0) = norm.x * norm.z * omc - norm.y * s;

  result(0, 1) = norm.x * norm.y * omc - norm.z * s;
  result(1, 1) = norm.y * norm.y * omc + c;
  result(2, 1) = norm.y * norm.z * omc + norm.x * s;

  result(0, 2) = norm.x * norm.z * omc + norm.y * s;
  result(1, 2) = norm.y * norm.z * omc - norm.x * s;
  result(2, 2) = norm.z * norm.z * omc + c;

  return result;
}

Matrix4 Matrix4::Scale(const Vector3& scale) {
  Matrix4 result;
  result.elements[0] = scale.x;
  result.elements[5] = scale.y;
  result.elements[10] = scale.z;
  return result;
}

Matrix4 Matrix4::Scale(float x, float y, float z) {
  return Scale(Vector3(x, y, z));
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top,
                              float near_clip, float far_clip) {
  Matrix4 result;

  result(0, 0) = 2.0f / (right - left);
  result(1, 1) = 2.0f / (top - bottom);
  result(2, 2) = -2.0f / (far_clip - near_clip);

  result(0, 3) = -(right + left) / (right - left);
  result(1, 3) = -(top + bottom) / (top - bottom);
  result(2, 3) = -(far_clip + near_clip) / (far_clip - near_clip);
  result(3, 3) = 1.0f;

  return result;
}

Matrix4 Matrix4::Perspective(float fov_radians, float aspect_ratio,
                             float near_clip, float far_clip) {
  Matrix4 result(0.0f);
  float tan_half_fov = std::tan(fov_radians / 2.0f);

  result(0, 0) = 1.0f / (aspect_ratio * tan_half_fov);
  result(1, 1) = 1.0f / tan_half_fov;
  result(2, 2) = -(far_clip + near_clip) / (far_clip - near_clip);
  result(3, 2) = -1.0f;
  result(2, 3) = -(2.0f * far_clip * near_clip) / (far_clip - near_clip);

  return result;
}

Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target,
                        const Vector3& up) {
  Vector3 forward = (target - eye).Normalized();
  Vector3 right = forward.Cross(up).Normalized();
  Vector3 true_up = right.Cross(forward);

  Matrix4 result;
  result(0, 0) = right.x;
  result(0, 1) = right.y;
  result(0, 2) = right.z;
  result(0, 3) = -right.Dot(eye);

  result(1, 0) = true_up.x;
  result(1, 1) = true_up.y;
  result(1, 2) = true_up.z;
  result(1, 3) = -true_up.Dot(eye);

  result(2, 0) = -forward.x;
  result(2, 1) = -forward.y;
  result(2, 2) = -forward.z;
  result(2, 3) = forward.Dot(eye);

  return result;
}

std::string Matrix4::ToString() const {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(3);
  for (std::size_t row = 0; row < 4; ++row) {
    ss << "[ ";
    for (std::size_t col = 0; col < 4; ++col) {
      ss << (*this)(row, col) << " ";
    }
    ss << "]\n";
  }
  return ss.str();
}

}  // namespace engine::math
