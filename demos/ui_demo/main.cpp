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
  props.title = "Agentic Game Engine - Interactive UI Demo";
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
  if (!font) {
    std::cerr << "Failed to load font." << std::endl;
    return 1;
  }

  engine::ui::UIContext ui;
  int click_count = 0;
  bool accent_alt = false;
  engine::ui::Rect card_bounds{520.0f, 180.0f, 180.0f, 220.0f};

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

    // Left control panel
    ui.Panel({40.0f, 60.0f, 380.0f, 420.0f},
             engine::renderer::Color(0.13f, 0.15f, 0.21f, 1.0f),
             engine::renderer::Color(0.35f, 0.40f, 0.52f, 1.0f), 2.0f);

    ui.Label("UI & Mouse Hit-Testing", {60.0f, 440.0f}, font,
             engine::renderer::Color::White());
    ui.Label("Clicks: " + std::to_string(click_count), {60.0f, 390.0f}, font,
             engine::renderer::Color::Cyan());

    if (ui.Button("inc", {60.0f, 310.0f, 220.0f, 48.0f}, "Increment (+1)",
                  font)) {
      ++click_count;
    }
    if (ui.Button("reset", {60.0f, 240.0f, 220.0f, 48.0f}, "Reset Counter",
                  font)) {
      click_count = 0;
    }
    if (ui.Button("color", {60.0f, 170.0f, 220.0f, 48.0f}, "Toggle Card Tint",
                  font)) {
      accent_alt = !accent_alt;
    }

    // Draggable card on the right
    engine::math::Vector2 drag_delta;
    const bool dragging = ui.IsDragging("card", card_bounds, &drag_delta);
    if (dragging) {
      card_bounds.x += drag_delta.x;
      card_bounds.y += drag_delta.y;
    }

    const engine::renderer::Color card_bg =
        accent_alt ? engine::renderer::Color(0.24f, 0.18f, 0.34f, 1.0f)
                   : engine::renderer::Color(0.16f, 0.26f, 0.24f, 1.0f);
    const engine::renderer::Color card_border =
        dragging ? engine::renderer::Color::Yellow()
                 : (ui.IsHovered(card_bounds)
                        ? engine::renderer::Color::Cyan()
                        : engine::renderer::Color(0.5f, 0.6f, 0.7f, 1.0f));

    ui.Panel(card_bounds, card_bg, card_border, 2.5f);
    ui.Label("Drag Me!", {card_bounds.x + 38.0f, card_bounds.y + 110.0f}, font,
             engine::renderer::Color::White());

    ui.EndFrame();
    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "UI demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
