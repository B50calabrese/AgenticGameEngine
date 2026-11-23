#include <gtest/gtest.h>

#include "engine/platform/window.h"

TEST(WindowTest, DefaultPropertiesMatchExpected) {
  engine::platform::WindowProps props;
  EXPECT_EQ(props.title, "Agentic Game Engine");
  EXPECT_EQ(props.width, 1280u);
  EXPECT_EQ(props.height, 720u);
  EXPECT_TRUE(props.vsync);
}

TEST(WindowTest, CustomPropertiesAssignment) {
  engine::platform::WindowProps props{"Custom Title", 1920, 1080, false};
  EXPECT_EQ(props.title, "Custom Title");
  EXPECT_EQ(props.width, 1920u);
  EXPECT_EQ(props.height, 1080u);
  EXPECT_FALSE(props.vsync);
}

TEST(WindowTest, WindowCreationHandling) {
  engine::platform::WindowProps props{"Test Window", 800, 600, false};
  auto window = engine::platform::Window::Create(props);

#if defined(ENGINE_ENABLE_GLFW)
  // If GLFW is enabled, creation succeeds when a display is available, or returns nullptr if headless
  if (window) {
    EXPECT_EQ(window->GetWidth(), 800u);
    EXPECT_EQ(window->GetHeight(), 600u);
    EXPECT_FALSE(window->ShouldClose());
    EXPECT_FALSE(window->IsVSync());
    EXPECT_NE(window->GetNativeWindow(), nullptr);
  }
#else
  // When built without GLFW, Window::Create cleanly returns nullptr
  EXPECT_EQ(window, nullptr);
#endif
}
