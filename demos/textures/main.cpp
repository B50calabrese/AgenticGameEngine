#include <cstdint>
#include <string>
#include <vector>

#include "engine/engine.h"

namespace {

using engine::core::Clock;
using engine::core::Timestep;
using engine::input::Input;
using engine::input::KeyCode;
using engine::math::Matrix4;
using engine::math::Vector2;
using engine::renderer::Color;
using engine::renderer::PrimitiveRenderer;
using engine::renderer::RenderCommand;
using engine::renderer::RenderQueue;
using engine::renderer::Texture2D;
using engine::renderer::TextureFormat;
using engine::renderer::TextureSpecification;

std::shared_ptr<Texture2D> CreateCheckerboardTexture(std::uint32_t size) {
  TextureSpecification spec;
  spec.width = size;
  spec.height = size;
  spec.format = TextureFormat::RGBA8;
  spec.generate_mips = false;

  std::vector<std::uint32_t> pixels(size * size);
  for (std::uint32_t y = 0; y < size; ++y) {
    for (std::uint32_t x = 0; x < size; ++x) {
      bool is_white = ((x + y) % 2) == 0;
      pixels[y * size + x] = is_white ? 0xFFFFFFFF : 0xFF333333;
    }
  }

  auto tex = Texture2D::Create(spec);
  tex->SetData(pixels.data(),
               static_cast<std::uint32_t>(pixels.size() * sizeof(std::uint32_t)));
  return tex;
}

std::shared_ptr<Texture2D> CreateGradientTexture(std::uint32_t size) {
  TextureSpecification spec;
  spec.width = size;
  spec.height = size;
  spec.format = TextureFormat::RGBA8;
  spec.generate_mips = false;

  std::vector<std::uint32_t> pixels(size * size);
  for (std::uint32_t y = 0; y < size; ++y) {
    for (std::uint32_t x = 0; x < size; ++x) {
      auto r = static_cast<std::uint8_t>((x * 255) / (size - 1));
      auto g = static_cast<std::uint8_t>((y * 255) / (size - 1));
      std::uint8_t b = 200;
      std::uint8_t a = 255;
      pixels[y * size + x] = (static_cast<std::uint32_t>(a) << 24) |
                             (static_cast<std::uint32_t>(b) << 16) |
                             (static_cast<std::uint32_t>(g) << 8) |
                             static_cast<std::uint32_t>(r);
    }
  }

  auto tex = Texture2D::Create(spec);
  tex->SetData(pixels.data(),
               static_cast<std::uint32_t>(pixels.size() * sizeof(std::uint32_t)));
  return tex;
}

}  // namespace

int main(int argc, char** argv) {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("================================================");
  ENGINE_LOG_INFO("   Agentic Game Engine - Textures & Z-Sort Demo ");
  ENGINE_LOG_INFO("   Press Esc to Quit                            ");
  ENGINE_LOG_INFO("================================================");

  int max_frames = 0;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--frames" && i + 1 < argc) {
      max_frames = std::stoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Textured Quads & RenderQueue";
  props.width = 960;
  props.height = 540;
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Could not create window for textures demo.");
    return 1;
  }

  if (!PrimitiveRenderer::Initialize()) {
    ENGINE_LOG_ERROR("Could not initialize PrimitiveRenderer.");
    return 1;
  }

  RenderCommand::SetViewport(0, 0, window->GetWidth(), window->GetHeight());

  auto checker_tex = CreateCheckerboardTexture(8);
  auto gradient_tex = CreateGradientTexture(64);
  RenderQueue render_queue;

  Clock clock;
  float angle = 0.0f;
  int frame_count = 0;

  while (!window->ShouldClose()) {
    Timestep dt = clock.Tick();
    if (Input::IsKeyPressed(KeyCode::Escape)) {
      break;
    }

    angle += 0.8f * dt.GetSeconds();

    RenderCommand::SetClearColor(Color(0.09f, 0.11f, 0.15f, 1.0f));
    RenderCommand::Clear();

    PrimitiveRenderer::ResetStats();

    auto ortho = Matrix4::Orthographic(
        0.0f, static_cast<float>(window->GetWidth()),
        0.0f, static_cast<float>(window->GetHeight()), -1.0f, 1.0f);

    // Submit out of Z-order to demonstrate RenderQueue Z-sorting:
    // Front-most (z = 2.0) submitted FIRST
    render_queue.SubmitTexturedQuad(
        Vector2(420.0f, 210.0f), Vector2(140.0f, 140.0f),
        checker_tex, Color::Cyan(), /*z_index=*/2.0f, angle);

    // Background (z = 0.0) submitted SECOND
    render_queue.SubmitTexturedQuad(
        Vector2(340.0f, 150.0f), Vector2(280.0f, 240.0f),
        gradient_tex, Color::White(), /*z_index=*/0.0f, 0.0f);

    // Middle layer (z = 1.0) submitted LAST
    render_queue.SubmitQuad(
        Vector2(390.0f, 190.0f), Vector2(180.0f, 180.0f),
        Color(0.95f, 0.25f, 0.35f, 0.85f), /*z_index=*/1.0f, -angle * 0.5f);

    // Side-by-side tinted checkerboards
    render_queue.SubmitTexturedQuad(
        Vector2(80.0f, 190.0f), Vector2(160.0f, 160.0f),
        checker_tex, Color::White(), 0.0f);
    render_queue.SubmitTexturedQuad(
        Vector2(720.0f, 190.0f), Vector2(160.0f, 160.0f),
        gradient_tex, Color::Yellow(), 0.0f, -angle);

    render_queue.Flush(ortho);

    window->OnUpdate();
    ++frame_count;

    if (max_frames > 0 && frame_count >= max_frames) {
      const auto& stats = PrimitiveRenderer::GetStats();
      ENGINE_LOG_INFO("Textures demo reached target frame limit: " + std::to_string(max_frames));
      ENGINE_LOG_INFO("Batch Stats: " + std::to_string(stats.draw_calls) +
                      " draw call(s), " + std::to_string(stats.quad_count) + " quads");
      break;
    }
  }

  PrimitiveRenderer::Shutdown();
  ENGINE_LOG_INFO("Textures demo exited cleanly.");
  return 0;
}
