#include <gtest/gtest.h>

#include "engine/ui/rect.h"
#include "engine/ui/ui_context.h"

namespace engine::ui {
namespace {

TEST(RectTest, ContainsAndOverlapsWorkCorrectly) {
  Rect r{10.0f, 20.0f, 100.0f, 50.0f};
  EXPECT_TRUE(r.Contains({10.0f, 20.0f}));
  EXPECT_TRUE(r.Contains({60.0f, 45.0f}));
  EXPECT_FALSE(r.Contains({9.9f, 45.0f}));
  EXPECT_FALSE(r.Contains({60.0f, 71.0f}));

  Rect overlapping{50.0f, 40.0f, 30.0f, 30.0f};
  Rect disjoint{200.0f, 200.0f, 10.0f, 10.0f};
  EXPECT_TRUE(r.Overlaps(overlapping));
  EXPECT_FALSE(r.Overlaps(disjoint));
}

TEST(UIContextTest, HoverClickAndDragTransitions) {
  UIContext ui;
  Rect btn{100.0f, 100.0f, 120.0f, 40.0f};

  // Frame 1: Hover without click
  ui.BeginFrame({150.0f, 120.0f}, false);
  EXPECT_TRUE(ui.IsHovered(btn));
  EXPECT_FALSE(ui.Button("play", btn, "Play", nullptr));
  ui.EndFrame();

  // Frame 2: Mouse press inside button
  ui.BeginFrame({150.0f, 120.0f}, true);
  EXPECT_FALSE(ui.Button("play", btn, "Play", nullptr));
  ui.EndFrame();

  // Frame 3: Mouse release inside button -> triggers click
  ui.BeginFrame({150.0f, 120.0f}, false);
  EXPECT_TRUE(ui.Button("play", btn, "Play", nullptr));
  ui.EndFrame();

  // Drag test
  ui.BeginFrame({110.0f, 110.0f}, true);
  engine::math::Vector2 delta;
  EXPECT_TRUE(ui.IsDragging("card", btn, &delta));
  ui.EndFrame();

  ui.BeginFrame({125.0f, 130.0f}, true);
  EXPECT_TRUE(ui.IsDragging("card", btn, &delta));
  EXPECT_FLOAT_EQ(delta.x, 15.0f);
  EXPECT_FLOAT_EQ(delta.y, 20.0f);
  ui.EndFrame();
}

}  // namespace
}  // namespace engine::ui
