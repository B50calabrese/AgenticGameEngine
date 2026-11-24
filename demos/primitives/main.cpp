#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <thread>

#include "engine/engine.h"
#include "engine/platform/window.h"
#include "engine/renderer/color.h"
#include "engine/renderer/primitive_renderer.h"
#include "engine/renderer/render_command.h"

int main(int argc, char** argv) {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("================================================");
  ENGINE_LOG_INFO("   Agentic Game Engine - Primitives Demo        ");
  ENGINE_LOG_INFO("   Engine Version: " + engine::GetVersionString());
  ENGINE_LOG_INFO("================================================");

  int max_frames = 0;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--frames" && i + 1 < argc) {
      max_frames = std::stoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - 2D Rendering Primitives";
  props.width = 960;
  props.height = 540;
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Could not create window for primitives demo.");
    return 1;
  }

  if (!engine::renderer::PrimitiveRenderer::Initialize()) {
    ENGINE_LOG_ERROR("Could not initialize PrimitiveRenderer.");
    return 1;
  }

  engine::renderer::RenderCommand::SetViewport(0, 0, window->GetWidth(),
                                              window->GetHeight());

  ENGINE_LOG_INFO("Rendering primitives demo running...");
  if (max_frames > 0) {
    ENGINE_LOG_INFO("Running for " + std::to_string(max_frames) + " frames...");
  } else {
    ENGINE_LOG_INFO("Close window to exit.");
  }

  float rotation = 0.0f;
  int frame_count = 0;

  while (!window->ShouldClose()) {
    // 1. Clear background to dark modern slate
    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.10f, 0.12f, 0.16f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    // 2. Begin 2D primitive batch (960x540 screen coordinate space)
    engine::renderer::PrimitiveRenderer::ResetStats();
    engine::renderer::PrimitiveRenderer::BeginBatch(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight()),
        /*y_down=*/false);

    // Grid lines / background axes
    engine::renderer::PrimitiveRenderer::DrawLine(
        0.0f, 270.0f, 960.0f, 270.0f,
        engine::renderer::Color(0.18f, 0.22f, 0.28f, 1.0f), 2.0f);
    engine::renderer::PrimitiveRenderer::DrawLine(
        480.0f, 0.0f, 480.0f, 540.0f,
        engine::renderer::Color(0.18f, 0.22f, 0.28f, 1.0f), 2.0f);

    // Left Quadrant: Static Quads & Outlines
    engine::renderer::PrimitiveRenderer::DrawQuad(
        60.0f, 320.0f, 140.0f, 100.0f,
        engine::renderer::Color::Red());
    engine::renderer::PrimitiveRenderer::DrawQuad(
        220.0f, 320.0f, 140.0f, 100.0f,
        engine::renderer::Color::Green());
    engine::renderer::PrimitiveRenderer::DrawRectOutline(
        50.0f, 310.0f, 320.0f, 120.0f,
        engine::renderer::Color::Yellow(), 2.0f);

    // Center: Animated Rotated Quad
    rotation += 0.02f;
    engine::renderer::PrimitiveRenderer::DrawRotatedQuad(
        480.0f, 270.0f, 120.0f, 120.0f, rotation,
        engine::renderer::Color::Cyan());
    engine::renderer::PrimitiveRenderer::DrawRotatedQuad(
        480.0f, 270.0f, 80.0f, 80.0f, -rotation * 1.5f,
        engine::renderer::Color::Magenta());

    // Right Quadrant: Circles
    engine::renderer::PrimitiveRenderer::DrawCircle(
        750.0f, 370.0f, 50.0f,
        engine::renderer::Color::Orange(), 40);
    engine::renderer::PrimitiveRenderer::DrawCircle(
        830.0f, 320.0f, 30.0f,
        engine::renderer::Color::Blue(), 32);

    // Bottom Left: Geometric Triangles
    engine::renderer::PrimitiveRenderer::DrawTriangle(
        100.0f, 80.0f, 200.0f, 80.0f, 150.0f, 200.0f,
        engine::renderer::Color(0.9f, 0.4f, 0.1f, 1.0f));
    engine::renderer::PrimitiveRenderer::DrawTriangle(
        220.0f, 80.0f, 320.0f, 80.0f, 270.0f, 200.0f,
        engine::renderer::Color(0.2f, 0.8f, 0.7f, 1.0f));

    // Bottom Right: Starburst Lines
    float center_x = 750.0f;
    float center_y = 140.0f;
    constexpr int kSpokes = 16;
    for (int s = 0; s < kSpokes; ++s) {
      float angle = (static_cast<float>(s) / kSpokes) * 6.2831853f;
      float ex = center_x + std::cos(angle) * 70.0f;
      float ey = center_y + std::sin(angle) * 70.0f;
      engine::renderer::PrimitiveRenderer::DrawLine(
          center_x, center_y, ex, ey,
          engine::renderer::Color(0.7f, 0.3f, 0.9f, 0.8f), 2.0f);
    }

    // End batch and flush to GPU
    engine::renderer::PrimitiveRenderer::EndBatch();

    // 3. Presentation
    window->OnUpdate();
    ++frame_count;

    if (max_frames > 0 && frame_count >= max_frames) {
      const auto& stats = engine::renderer::PrimitiveRenderer::GetStats();
      ENGINE_LOG_INFO("Reached target frame limit: " + std::to_string(max_frames));
      ENGINE_LOG_INFO("Batch Stats: " + std::to_string(stats.draw_calls) +
                      " draw call(s), " + std::to_string(stats.quad_count) +
                      " quads, " + std::to_string(stats.triangle_count) +
                      " triangles, " + std::to_string(stats.line_count) + " lines");
      break;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  ENGINE_LOG_INFO("Primitives demo exited cleanly after " +
                  std::to_string(frame_count) + " frames.");
  return 0;
}
