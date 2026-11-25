#include "engine/math/transform.h"

namespace engine::math {

Matrix4 Transform::ToMatrix() const {
  return Matrix4::Translation(position) *
         Matrix4::RotationZ(rotation.z) *
         Matrix4::RotationY(rotation.y) *
         Matrix4::RotationX(rotation.x) *
         Matrix4::Scale(scale);
}

Matrix4 Transform::ToInverseMatrix() const {
  Vector3 inv_scale(
      (std::abs(scale.x) > kEpsilon) ? (1.0f / scale.x) : 0.0f,
      (std::abs(scale.y) > kEpsilon) ? (1.0f / scale.y) : 0.0f,
      (std::abs(scale.z) > kEpsilon) ? (1.0f / scale.z) : 0.0f);

  return Matrix4::Scale(inv_scale) *
         Matrix4::RotationX(-rotation.x) *
         Matrix4::RotationY(-rotation.y) *
         Matrix4::RotationZ(-rotation.z) *
         Matrix4::Translation(-position);
}

}  // namespace engine::math
