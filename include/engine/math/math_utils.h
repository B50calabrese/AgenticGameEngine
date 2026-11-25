#ifndef ENGINE_MATH_MATH_UTILS_H_
#define ENGINE_MATH_MATH_UTILS_H_

#include <cmath>

namespace engine::math {

inline constexpr float kPi = 3.14159265358979323846f;
inline constexpr float kTwoPi = 6.28318530717958647692f;
inline constexpr float kHalfPi = 1.57079632679489661923f;
inline constexpr float kDegToRad = kPi / 180.0f;
inline constexpr float kRadToDeg = 180.0f / kPi;
inline constexpr float kEpsilon = 1e-6f;

constexpr float ToRadians(float degrees) {
  return degrees * kDegToRad;
}

constexpr float ToDegrees(float radians) {
  return radians * kRadToDeg;
}

template <typename T>
constexpr T Clamp(T value, T min_value, T max_value) {
  return (value < min_value) ? min_value : ((value > max_value) ? max_value : value);
}

template <typename T>
constexpr T Lerp(T a, T b, float t) {
  return a + (b - a) * t;
}

inline bool Approximately(float a, float b, float epsilon = kEpsilon) {
  return std::abs(a - b) <= epsilon;
}

}  // namespace engine::math

#endif  // ENGINE_MATH_MATH_UTILS_H_
