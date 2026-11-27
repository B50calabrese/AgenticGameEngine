#ifndef ENGINE_CORE_TIMESTEP_H_
#define ENGINE_CORE_TIMESTEP_H_

namespace engine::core {

/**
 * @brief Represents frame delta time with conversions between seconds and milliseconds.
 */
class Timestep {
 public:
  constexpr explicit Timestep(float time_seconds = 0.0f)
      : time_seconds_(time_seconds) {}

  constexpr operator float() const { return time_seconds_; }

  constexpr float GetSeconds() const { return time_seconds_; }
  constexpr float GetMilliseconds() const { return time_seconds_ * 1000.0f; }

 private:
  float time_seconds_ = 0.0f;
};

}  // namespace engine::core

#endif  // ENGINE_CORE_TIMESTEP_H_
