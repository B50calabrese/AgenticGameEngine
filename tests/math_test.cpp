#include <gtest/gtest.h>

#include "engine/math/math_utils.h"
#include "engine/math/matrix4.h"
#include "engine/math/transform.h"
#include "engine/math/vector2.h"
#include "engine/math/vector3.h"
#include "engine/math/vector4.h"

namespace {

using engine::math::Approximately;
using engine::math::Clamp;
using engine::math::kDegToRad;
using engine::math::kHalfPi;
using engine::math::kPi;
using engine::math::kRadToDeg;
using engine::math::kTwoPi;
using engine::math::Lerp;
using engine::math::Matrix4;
using engine::math::ToDegrees;
using engine::math::ToRadians;
using engine::math::Transform;
using engine::math::Vector2;
using engine::math::Vector3;
using engine::math::Vector4;

// =============================================================================
// MathUtils Tests
// =============================================================================

TEST(MathUtilsTest, RadiansAndDegreesConversion) {
  EXPECT_NEAR(ToRadians(180.0f), kPi, 1e-5f);
  EXPECT_NEAR(ToRadians(90.0f), kHalfPi, 1e-5f);
  EXPECT_NEAR(ToDegrees(kPi), 180.0f, 1e-4f);
  EXPECT_NEAR(ToDegrees(kHalfPi), 90.0f, 1e-4f);
}

TEST(MathUtilsTest, ClampAndLerp) {
  EXPECT_FLOAT_EQ(Clamp(5.0f, 0.0f, 10.0f), 5.0f);
  EXPECT_FLOAT_EQ(Clamp(-2.0f, 0.0f, 10.0f), 0.0f);
  EXPECT_FLOAT_EQ(Clamp(15.0f, 0.0f, 10.0f), 10.0f);

  EXPECT_FLOAT_EQ(Lerp(0.0f, 100.0f, 0.5f), 50.0f);
  EXPECT_FLOAT_EQ(Lerp(10.0f, 20.0f, 0.25f), 12.5f);
}

// =============================================================================
// Vector2 Tests
// =============================================================================

TEST(Vector2Test, DefaultAndParamConstructors) {
  Vector2 v1;
  EXPECT_FLOAT_EQ(v1.x, 0.0f);
  EXPECT_FLOAT_EQ(v1.y, 0.0f);

  Vector2 v2(3.0f, 4.0f);
  EXPECT_FLOAT_EQ(v2.x, 3.0f);
  EXPECT_FLOAT_EQ(v2.y, 4.0f);
  EXPECT_FLOAT_EQ(v2[0], 3.0f);
  EXPECT_FLOAT_EQ(v2[1], 4.0f);
}

TEST(Vector2Test, ArithmeticOperations) {
  Vector2 a(2.0f, 5.0f);
  Vector2 b(1.0f, 3.0f);

  Vector2 add = a + b;
  EXPECT_EQ(add, Vector2(3.0f, 8.0f));

  Vector2 sub = a - b;
  EXPECT_EQ(sub, Vector2(1.0f, 2.0f));

  Vector2 mul = a * 2.0f;
  EXPECT_EQ(mul, Vector2(4.0f, 10.0f));

  Vector2 div = a / 2.0f;
  EXPECT_EQ(div, Vector2(1.0f, 2.5f));

  Vector2 neg = -a;
  EXPECT_EQ(neg, Vector2(-2.0f, -5.0f));
}

TEST(Vector2Test, LengthAndNormalization) {
  Vector2 v(3.0f, 4.0f);
  EXPECT_FLOAT_EQ(v.LengthSquared(), 25.0f);
  EXPECT_FLOAT_EQ(v.Length(), 5.0f);

  Vector2 norm = v.Normalized();
  EXPECT_NEAR(norm.Length(), 1.0f, 1e-5f);
  EXPECT_FLOAT_EQ(norm.x, 0.6f);
  EXPECT_FLOAT_EQ(norm.y, 0.8f);

  Vector2 zero(0.0f, 0.0f);
  EXPECT_EQ(zero.Normalized(), Vector2(0.0f, 0.0f));
}

TEST(Vector2Test, DotAndCrossProducts) {
  Vector2 a(1.0f, 0.0f);
  Vector2 b(0.0f, 1.0f);

  EXPECT_FLOAT_EQ(a.Dot(b), 0.0f);  // Orthogonal
  EXPECT_FLOAT_EQ(a.Cross(b), 1.0f);

  Vector2 c(2.0f, 3.0f);
  Vector2 d(4.0f, -1.0f);
  EXPECT_FLOAT_EQ(c.Dot(d), 5.0f);
}

TEST(Vector2Test, DistanceAndAngle) {
  Vector2 p1(1.0f, 1.0f);
  Vector2 p2(4.0f, 5.0f);

  EXPECT_FLOAT_EQ(p1.Distance(p2), 5.0f);
  EXPECT_FLOAT_EQ(p1.DistanceSquared(p2), 25.0f);

  Vector2 up(0.0f, 1.0f);
  EXPECT_NEAR(up.Angle(), kHalfPi, 1e-5f);

  Vector2 perp = up.Perpendicular();
  EXPECT_EQ(perp, Vector2(-1.0f, 0.0f));
}

// =============================================================================
// Vector3 Tests
// =============================================================================

TEST(Vector3Test, BasicOperationsAndCross) {
  Vector3 a(1.0f, 0.0f, 0.0f);
  Vector3 b(0.0f, 1.0f, 0.0f);

  Vector3 c = a.Cross(b);
  EXPECT_EQ(c, Vector3(0.0f, 0.0f, 1.0f));  // X x Y = Z

  Vector3 d(2.0f, 3.0f, 4.0f);
  EXPECT_FLOAT_EQ(d.LengthSquared(), 4.0f + 9.0f + 16.0f);
  EXPECT_NEAR(d.Normalized().Length(), 1.0f, 1e-5f);
}

// =============================================================================
// Matrix4 Tests
// =============================================================================

TEST(Matrix4Test, IdentityIsDiagonalOnes) {
  Matrix4 identity = Matrix4::Identity();
  for (std::size_t row = 0; row < 4; ++row) {
    for (std::size_t col = 0; col < 4; ++col) {
      if (row == col) {
        EXPECT_FLOAT_EQ(identity(row, col), 1.0f);
      } else {
        EXPECT_FLOAT_EQ(identity(row, col), 0.0f);
      }
    }
  }
}

TEST(Matrix4Test, TranslationTransformsPoint) {
  Matrix4 t = Matrix4::Translation(Vector3(10.0f, -5.0f, 3.0f));
  Vector3 p(1.0f, 2.0f, 3.0f);

  Vector3 result = t.TransformPoint(p);
  EXPECT_FLOAT_EQ(result.x, 11.0f);
  EXPECT_FLOAT_EQ(result.y, -3.0f);
  EXPECT_FLOAT_EQ(result.z, 6.0f);

  // Directions are not affected by translation
  Vector3 dir = t.TransformDirection(Vector3(1.0f, 0.0f, 0.0f));
  EXPECT_EQ(dir, Vector3(1.0f, 0.0f, 0.0f));
}

TEST(Matrix4Test, RotationZTransforms2DCoordinates) {
  Matrix4 r = Matrix4::RotationZ(kHalfPi);  // 90 degrees CCW
  Vector3 p(1.0f, 0.0f, 0.0f);

  Vector3 result = r.TransformPoint(p);
  EXPECT_NEAR(result.x, 0.0f, 1e-5f);
  EXPECT_NEAR(result.y, 1.0f, 1e-5f);
  EXPECT_NEAR(result.z, 0.0f, 1e-5f);
}

TEST(Matrix4Test, ScaleMultipliesCoordinates) {
  Matrix4 s = Matrix4::Scale(Vector3(2.0f, 3.0f, 4.0f));
  Vector3 p(2.0f, 2.0f, 2.0f);

  Vector3 result = s.TransformPoint(p);
  EXPECT_FLOAT_EQ(result.x, 4.0f);
  EXPECT_FLOAT_EQ(result.y, 6.0f);
  EXPECT_FLOAT_EQ(result.z, 8.0f);
}

TEST(Matrix4Test, MatrixMultiplicationAssociative) {
  Matrix4 t = Matrix4::Translation(1.0f, 2.0f, 3.0f);
  Matrix4 r = Matrix4::RotationZ(0.5f);
  Matrix4 s = Matrix4::Scale(2.0f, 2.0f, 2.0f);

  Matrix4 m1 = (t * r) * s;
  Matrix4 m2 = t * (r * s);

  EXPECT_EQ(m1, m2);
}

TEST(Matrix4Test, InversionReversesTransformation) {
  Matrix4 t = Matrix4::Translation(Vector3(5.0f, -3.0f, 12.0f));
  Matrix4 r = Matrix4::RotationZ(0.785f);
  Matrix4 s = Matrix4::Scale(Vector3(2.0f, 3.0f, 0.5f));

  Matrix4 transform = t * r * s;
  Matrix4 inverse = transform.Inverted();

  Matrix4 identity = transform * inverse;
  EXPECT_EQ(identity, Matrix4::Identity());

  Vector3 point(10.0f, 20.0f, 30.0f);
  Vector3 transformed = transform.TransformPoint(point);
  Vector3 back = inverse.TransformPoint(transformed);

  EXPECT_NEAR(back.x, point.x, 1e-4f);
  EXPECT_NEAR(back.y, point.y, 1e-4f);
  EXPECT_NEAR(back.z, point.z, 1e-4f);
}

TEST(Matrix4Test, OrthographicProjectionMapsBoundsToNDC) {
  // 960x540 viewport: Left=0, Right=960, Bottom=0, Top=540, Near=-1, Far=1
  Matrix4 ortho = Matrix4::Orthographic(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

  // Bottom-Left (0, 0, 0) -> (-1, -1, 0)
  Vector3 bl = ortho.TransformPoint(Vector3(0.0f, 0.0f, 0.0f));
  EXPECT_NEAR(bl.x, -1.0f, 1e-5f);
  EXPECT_NEAR(bl.y, -1.0f, 1e-5f);

  // Top-Right (960, 540, 0) -> (+1, +1, 0)
  Vector3 tr = ortho.TransformPoint(Vector3(960.0f, 540.0f, 0.0f));
  EXPECT_NEAR(tr.x, 1.0f, 1e-5f);
  EXPECT_NEAR(tr.y, 1.0f, 1e-5f);

  // Center (480, 270, 0) -> (0, 0, 0)
  Vector3 center = ortho.TransformPoint(Vector3(480.0f, 270.0f, 0.0f));
  EXPECT_NEAR(center.x, 0.0f, 1e-5f);
  EXPECT_NEAR(center.y, 0.0f, 1e-5f);
}

// =============================================================================
// Transform Tests
// =============================================================================

TEST(TransformTest, 2DConvenienceMethods) {
  Transform t;
  t.SetPosition2D(100.0f, 200.0f);
  t.SetRotation2D(kHalfPi);
  t.SetScale2D(2.0f, 3.0f);

  EXPECT_EQ(t.GetPosition2D(), Vector2(100.0f, 200.0f));
  EXPECT_FLOAT_EQ(t.GetRotation2D(), kHalfPi);
  EXPECT_EQ(t.GetScale2D(), Vector2(2.0f, 3.0f));

  Matrix4 mat = t.ToMatrix();
  Matrix4 inv = t.ToInverseMatrix();

  Vector3 point(10.0f, 5.0f, 0.0f);
  Vector3 transformed = mat.TransformPoint(point);
  Vector3 restored = inv.TransformPoint(transformed);

  EXPECT_NEAR(restored.x, point.x, 1e-4f);
  EXPECT_NEAR(restored.y, point.y, 1e-4f);
}

}  // namespace
