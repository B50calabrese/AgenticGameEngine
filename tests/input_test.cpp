#include <gtest/gtest.h>
#include <thread>

#include "engine/core/clock.h"
#include "engine/core/timestep.h"
#include "engine/input/input.h"
#include "engine/input/key_codes.h"
#include "engine/input/mouse_button_codes.h"

namespace {

using engine::core::Clock;
using engine::core::Timestep;
using engine::input::Input;
using engine::input::KeyCode;
using engine::input::MouseButton;

TEST(TimestepTest, SecondAndMillisecondConversions) {
  Timestep ts(0.016f);
  EXPECT_FLOAT_EQ(ts.GetSeconds(), 0.016f);
  EXPECT_FLOAT_EQ(ts.GetMilliseconds(), 16.0f);
  EXPECT_FLOAT_EQ(static_cast<float>(ts), 0.016f);
}

TEST(ClockTest, TickMeasuresElapsedTime) {
  Clock clock;
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  Timestep dt = clock.Tick();

  EXPECT_GT(dt.GetSeconds(), 0.005f);
  EXPECT_GT(clock.GetTotalElapsedSeconds(), 0.005f);
}

TEST(KeyCodeTest, KeyCodeValuesMatchExpected) {
  EXPECT_EQ(static_cast<int>(KeyCode::Space), 32);
  EXPECT_EQ(static_cast<int>(KeyCode::A), 65);
  EXPECT_EQ(static_cast<int>(KeyCode::Escape), 256);
  EXPECT_EQ(static_cast<int>(KeyCode::Left), 263);
  EXPECT_EQ(static_cast<int>(KeyCode::Right), 262);
  EXPECT_EQ(static_cast<int>(MouseButton::Left), 0);
  EXPECT_EQ(static_cast<int>(MouseButton::Right), 1);
}

TEST(InputTest, FallbackReturnsFalseWhenNoWindow) {
  Input::SetTargetWindow(nullptr);
  EXPECT_FALSE(Input::IsKeyPressed(KeyCode::Space));
  EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButton::Left));
  auto mouse_pos = Input::GetMousePosition();
  EXPECT_FLOAT_EQ(mouse_pos.x, 0.0f);
  EXPECT_FLOAT_EQ(mouse_pos.y, 0.0f);
}

}  // namespace
