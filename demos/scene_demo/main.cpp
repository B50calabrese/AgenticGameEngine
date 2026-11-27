#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "engine/engine.h"
#include "engine/renderer/font.h"

class MenuScene : public engine::core::Scene {
 public:
  MenuScene(engine::core::SceneManager* manager,
            std::shared_ptr<engine::renderer::Font> font)
      : manager_(manager),
        font_(std::move(font)),
        panel_y_(-200.0f, 180.0f, 1.0f, engine::core::easing::EaseOutBack) {}

  void OnEnter() override { panel_y_.Reset(); }

  void OnUpdate(engine::core::Timestep dt) override {
    panel_y_.Update(dt.GetSeconds());
    if (engine::input::Input::IsKeyPressed(engine::input::KeyCode::Space)) {
      manager_->SwitchTo("game");
    }
  }

  void OnRender() override {
    const float y = panel_y_.GetValue();
    engine::renderer::PrimitiveRenderer::DrawQuad(
        280.0f, y, 400.0f, 180.0f,
        engine::renderer::Color(0.16f, 0.20f, 0.28f, 1.0f));
    if (font_) {
      font_->DrawString("MENU SCENE (EaseOutBack)", {310.0f, y + 110.0f},
                        engine::renderer::Color::White());
      font_->DrawString("Press SPACE for Game Scene", {310.0f, y + 60.0f},
                        engine::renderer::Color::Cyan());
    }
  }

 private:
  engine::core::SceneManager* manager_;
  std::shared_ptr<engine::renderer::Font> font_;
  engine::core::Tween<float> panel_y_;
};

class GameScene : public engine::core::Scene {
 public:
  GameScene(engine::core::SceneManager* manager,
            std::shared_ptr<engine::renderer::Font> font)
      : manager_(manager),
        font_(std::move(font)),
        circle_x_(160.0f, 800.0f, 2.0f,
                  engine::core::easing::EaseInOutCubic) {}

  void OnEnter() override {
    circle_x_.Reset();
    forward_ = true;
  }

  void OnUpdate(engine::core::Timestep dt) override {
    circle_x_.Update(dt.GetSeconds());
    if (circle_x_.IsFinished()) {
      circle_x_.Reset();
      forward_ = !forward_;
    }
    if (engine::input::Input::IsKeyPressed(engine::input::KeyCode::Escape)) {
      manager_->SwitchTo("menu");
    }
  }

  void OnRender() override {
    const float x = forward_ ? circle_x_.GetValue()
                             : (960.0f - circle_x_.GetValue());
    engine::renderer::PrimitiveRenderer::DrawCircle(
        x, 270.0f, 50.0f, engine::renderer::Color::Yellow());
    if (font_) {
      font_->DrawString("GAME SCENE (EaseInOutCubic)", {80.0f, 460.0f},
                        engine::renderer::Color::White());
      font_->DrawString("Press ESC for Menu Scene", {80.0f, 420.0f},
                        engine::renderer::Color::Green());
    }
  }

 private:
  engine::core::SceneManager* manager_;
  std::shared_ptr<engine::renderer::Font> font_;
  engine::core::Tween<float> circle_x_;
  bool forward_ = true;
};

int main(int argc, char** argv) {
  int max_frames = -1;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--frames" && i + 1 < argc) {
      max_frames = std::atoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Scene & Tweening Demo";
  props.width = 960;
  props.height = 540;
  auto window = engine::platform::Window::Create(props);
  if (!window) {
    std::cerr << "Failed to create window." << std::endl;
    return 1;
  }

  engine::renderer::PrimitiveRenderer::Initialize();
  engine::renderer::RenderCommand::SetViewport(0, 0, window->GetWidth(),
                                               window->GetHeight());

  auto font = engine::renderer::Font::Load(
      "thirdparty/fonts/Quicksand-Regular.ttf", 24);

  engine::core::SceneManager scenes;
  scenes.RegisterScene("menu", std::make_unique<MenuScene>(&scenes, font));
  scenes.RegisterScene("game", std::make_unique<GameScene>(&scenes, font));
  scenes.SwitchTo("menu");

  engine::core::Clock clock;
  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    const engine::core::Timestep dt = clock.Tick();
    scenes.Update(dt);

    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::BeginBatch(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight()));

    scenes.Render();

    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "Scene demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
