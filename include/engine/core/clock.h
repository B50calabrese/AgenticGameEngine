#ifndef ENGINE_CORE_CLOCK_H_
#define ENGINE_CORE_CLOCK_H_

#include <chrono>

#include "engine/core/timestep.h"

namespace engine::core {

/**
 * @brief High-precision timer for frame delta time and total elapsed time tracking.
 */
class Clock {
 public:
  Clock();

  /**
   * @brief Advances the clock and returns the elapsed time since the previous tick.
   */
  Timestep Tick();

  /**
   * @brief Resets the tick baseline without modifying total elapsed start time.
   */
  void ResetTick();

  /**
   * @brief Returns total time in seconds since the clock was created or reset.
   */
  float GetTotalElapsedSeconds() const;

 private:
  using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

  TimePoint start_time_;
  TimePoint last_tick_time_;
};

}  // namespace engine::core

#endif  // ENGINE_CORE_CLOCK_H_
