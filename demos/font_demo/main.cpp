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
  props.title = "Agentic Game Engine - Font & Text Demo";
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

  const std::string font_path = "thirdparty/fonts/Quicksand-Regular.ttf";
  auto font_large = engine::renderer::Font::Load(font_path, 48);
  auto font_small = engine::renderer::Font::Load(font_path, 24);
  if (!font_large || !font_small) {
    std::cerr << "Failed to load font: " << font_path << std::endl;
    return 1;
  }

  std::cout << "Loaded font from " << font_path << " (atlas size "
            << font_large->GetAtlasTexture()->GetWidth() << "x"
            << font_large->GetAtlasTexture()->GetHeight() << ")" << std::endl;

  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::ResetStats();
    engine::renderer::PrimitiveRenderer::BeginBatch(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight()));

    font_large->DrawString("Agentic Game Engine", {80.0f, 400.0f},
                           engine::renderer::Color::White());
    font_small->DrawString("TrueType Font & Glyph Atlas Rendering (REND-04)",
                           {80.0f, 340.0f}, engine::renderer::Color::Cyan());
    font_small->DrawString("ABCDEFGHIJKLMNOPQRSTUVWXYZ 0123456789 !@#$%",
                           {80.0f, 260.0f}, engine::renderer::Color::Yellow());
    font_small->DrawString("Rotated Text!", {600.0f, 160.0f},
                           engine::renderer::Color::Green(), 0.25f);

    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "Font demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
