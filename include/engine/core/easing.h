#ifndef ENGINE_CORE_EASING_H_
#define ENGINE_CORE_EASING_H_

#include <algorithm>
#include <cmath>
#include <functional>

namespace engine::core {

/**
 * @brief Standard normalized [0, 1] easing functions.
 */
namespace easing {

inline float Linear(float t) { return t; }

inline float EaseInQuad(float t) { return t * t; }

inline float EaseOutQuad(float t) { return t * (2.0f - t); }

inline float EaseInOutQuad(float t) {
  return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}

inline float EaseOutCubic(float t) {
  const float f = t - 1.0f;
  return f * f * f + 1.0f;
}

inline float EaseInOutCubic(float t) {
  return t < 0.5f ? 4.0f * t * t * t
                  : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}

inline float EaseOutBack(float t) {
  constexpr float c1 = 1.70158f;
  constexpr float c3 = c1 + 1.0f;
  const float f = t - 1.0f;
  return 1.0f + c3 * f * f * f + c1 * f * f;
}

}  // namespace easing

/**
 * @brief Lightweight value tweening utility.
 */
template <typename T>
class Tween {
 public:
  using EaseFn = std::function<float(float)>;

  Tween(T start, T end, float duration_seconds, EaseFn ease_fn = easing::Linear)
      : start_(start),
        end_(end),
        duration_(duration_seconds > 0.0f ? duration_seconds : 0.0001f),
        ease_fn_(std::move(ease_fn)) {}

  void Update(float delta_seconds) {
    if (elapsed_ < duration_) {
      elapsed_ = std::min(elapsed_ + delta_seconds, duration_);
    }
  }

  T GetValue() const {
    const float t = std::clamp(elapsed_ / duration_, 0.0f, 1.0f);
    const float eased = ease_fn_ ? ease_fn_(t) : t;
    return start_ + (end_ - start_) * eased;
  }

  bool IsFinished() const { return elapsed_ >= duration_; }

  void Reset() { elapsed_ = 0.0f; }

 private:
  T start_;
  T end_;
  float duration_;
  float elapsed_ = 0.0f;
  EaseFn ease_fn_;
};

}  // namespace engine::core

#endif  // ENGINE_CORE_EASING_H_
