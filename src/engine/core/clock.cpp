#include "engine/core/clock.h"

namespace engine::core {

Clock::Clock()
    : start_time_(std::chrono::high_resolution_clock::now()),
      last_tick_time_(start_time_) {}

Timestep Clock::Tick() {
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> delta = current_time - last_tick_time_;
  last_tick_time_ = current_time;
  return Timestep(delta.count());
}

void Clock::ResetTick() {
  last_tick_time_ = std::chrono::high_resolution_clock::now();
}

float Clock::GetTotalElapsedSeconds() const {
  auto current_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> elapsed = current_time - start_time_;
  return elapsed.count();
}

}  // namespace engine::core
