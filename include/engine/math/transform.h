#ifndef ENGINE_MATH_TRANSFORM_H_
#define ENGINE_MATH_TRANSFORM_H_

#include "engine/math/matrix4.h"
#include "engine/math/vector2.h"
#include "engine/math/vector3.h"

namespace engine::math {

/**
 * @brief Represents spatial translation, rotation (Euler angles), and scale.
 *
 * Supports both 2D and 3D coordinate transformations.
 */
class Transform {
 public:
  Vector3 position = Vector3::Zero();
  Vector3 rotation = Vector3::Zero();  // Euler angles in radians (X=pitch, Y=yaw, Z=roll/2D rotation)
  Vector3 scale = Vector3::One();

  Transform() = default;
  Transform(const Vector3& pos, const Vector3& rot, const Vector3& scl)
      : position(pos), rotation(rot), scale(scl) {}

  // 2D convenience methods
  void SetPosition2D(float x, float y) {
    position.x = x;
    position.y = y;
  }

  Vector2 GetPosition2D() const {
    return Vector2(position.x, position.y);
  }

  void SetRotation2D(float radians) {
    rotation.z = radians;
  }

  float GetRotation2D() const {
    return rotation.z;
  }

  void SetScale2D(float x, float y) {
    scale.x = x;
    scale.y = y;
  }

  Vector2 GetScale2D() const {
    return Vector2(scale.x, scale.y);
  }

  void Translate(const Vector3& delta) {
    position += delta;
  }

  void Translate(const Vector2& delta) {
    position.x += delta.x;
    position.y += delta.y;
  }

  void Rotate2D(float delta_radians) {
    rotation.z += delta_radians;
  }

  /**
   * @brief Calculates the local transform matrix M = T * R * S.
   */
  Matrix4 ToMatrix() const;

  /**
   * @brief Calculates the inverse local transform matrix M^-1.
   */
  Matrix4 ToInverseMatrix() const;

  bool operator==(const Transform& other) const {
    return position == other.position && rotation == other.rotation &&
           scale == other.scale;
  }

  bool operator!=(const Transform& other) const {
    return !(*this == other);
  }
};

}  // namespace engine::math

#endif  // ENGINE_MATH_TRANSFORM_H_
