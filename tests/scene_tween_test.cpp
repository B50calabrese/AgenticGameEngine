#include <gtest/gtest.h>

#include "engine/core/easing.h"
#include "engine/core/scene.h"

namespace engine::core {
namespace {

class MockScene : public Scene {
 public:
  void OnEnter() override { ++enter_count; }
  void OnExit() override { ++exit_count; }
  void OnUpdate(Timestep dt) override { total_dt += dt.GetSeconds(); }
  void OnRender() override { ++render_count; }

  int enter_count = 0;
  int exit_count = 0;
  int render_count = 0;
  float total_dt = 0.0f;
};

TEST(EasingTest, TweenInterpolatesAndFinishes) {
  Tween<float> tween(0.0f, 100.0f, 2.0f, easing::Linear);
  EXPECT_FLOAT_EQ(tween.GetValue(), 0.0f);
  EXPECT_FALSE(tween.IsFinished());

  tween.Update(1.0f);
  EXPECT_FLOAT_EQ(tween.GetValue(), 50.0f);
  EXPECT_FALSE(tween.IsFinished());

  tween.Update(1.5f);
  EXPECT_FLOAT_EQ(tween.GetValue(), 100.0f);
  EXPECT_TRUE(tween.IsFinished());

  tween.Reset();
  EXPECT_FLOAT_EQ(tween.GetValue(), 0.0f);
  EXPECT_FALSE(tween.IsFinished());
}

TEST(SceneManagerTest, TransitionsAndLifecycleHooks) {
  SceneManager manager;
  auto menu = std::make_unique<MockScene>();
  auto game = std::make_unique<MockScene>();
  MockScene* menu_ptr = menu.get();
  MockScene* game_ptr = game.get();

  manager.RegisterScene("menu", std::move(menu));
  manager.RegisterScene("game", std::move(game));

  EXPECT_FALSE(manager.SwitchTo("unknown"));
  EXPECT_TRUE(manager.SwitchTo("menu"));
  EXPECT_EQ(manager.GetCurrentSceneName(), "menu");
  EXPECT_EQ(menu_ptr->enter_count, 1);

  manager.Update(Timestep(0.5f));
  manager.Render();
  EXPECT_FLOAT_EQ(menu_ptr->total_dt, 0.5f);
  EXPECT_EQ(menu_ptr->render_count, 1);

  EXPECT_TRUE(manager.SwitchTo("game"));
  EXPECT_EQ(menu_ptr->exit_count, 1);
  EXPECT_EQ(game_ptr->enter_count, 1);
  EXPECT_EQ(manager.GetCurrentSceneName(), "game");
}

}  // namespace
}  // namespace engine::core
