#include <cstdlib>
#include <iostream>
#include <string>

#include "engine/engine.h"
#include "engine/renderer/font.h"

int main(int argc, char** argv) {
  int max_frames = -1;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--frames" && i + 1 < argc) {
      max_frames = std::atoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Save State Demo";
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
      "thirdparty/fonts/Quicksand-Regular.ttf", 22);

  int gold = 100;
  std::string status = "Ready";
  const std::string save_path = "/tmp/age_save_demo.sav";
  engine::ui::UIContext ui;

  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    const float w = static_cast<float>(window->GetWidth());
    const float h = static_cast<float>(window->GetHeight());

    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::BeginBatch(w, h);
    ui.BeginFrame(h);

    ui.Panel({60.0f, 80.0f, 420.0f, 380.0f},
             engine::renderer::Color(0.13f, 0.15f, 0.21f, 1.0f),
             engine::renderer::Color(0.35f, 0.40f, 0.52f, 1.0f), 2.0f);

    ui.Label("Save State Demo", {80.0f, 420.0f}, font,
             engine::renderer::Color::White());
    ui.Label("Gold: " + std::to_string(gold), {80.0f, 375.0f}, font,
             engine::renderer::Color::Yellow());
    ui.Label("Status: " + status, {80.0f, 340.0f}, font,
             engine::renderer::Color::Cyan());

    if (ui.Button("add_gold", {80.0f, 260.0f, 240.0f, 44.0f}, "Add +50 Gold",
                  font)) {
      gold += 50;
      status = "Gold updated";
    }
    if (ui.Button("save", {80.0f, 200.0f, 240.0f, 44.0f}, "Save State", font)) {
      engine::serialization::SaveState save;
      save.SetInt("gold", gold);
      if (save.SaveToFile(save_path)) {
        status = "Saved to " + save_path;
      }
    }
    if (ui.Button("load", {80.0f, 140.0f, 240.0f, 44.0f}, "Load State", font)) {
      engine::serialization::SaveState loaded;
      if (loaded.LoadFromFile(save_path)) {
        gold = loaded.GetInt("gold", gold);
        status = "Loaded from " + save_path;
      }
    }

    ui.EndFrame();
    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "Save demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
