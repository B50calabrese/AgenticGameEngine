#include <cstdlib>
#include <iostream>
#include <string>

#include "engine/engine.h"

struct Transform2D {
  float x = 0.0f;
  float y = 0.0f;
  float size = 16.0f;
};

struct Velocity2D {
  float vx = 0.0f;
  float vy = 0.0f;
};

struct ColorComp {
  engine::renderer::Color color;
};

int main(int argc, char** argv) {
  int max_frames = -1;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--frames" && i + 1 < argc) {
      max_frames = std::atoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - ECS Demo (60 Bouncing Entities)";
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

  engine::ecs::Registry registry;
  for (int i = 0; i < 60; ++i) {
    auto e = registry.CreateEntity();
    registry.AddComponent<Transform2D>(
        e, 40.0f + static_cast<float>((i * 37) % 880),
        40.0f + static_cast<float>((i * 53) % 460),
        12.0f + static_cast<float>((i % 4) * 6));
    registry.AddComponent<Velocity2D>(
        e, (i % 2 == 0 ? 1.0f : -1.0f) * (80.0f + (i % 5) * 30.0f),
        (i % 3 == 0 ? 1.0f : -1.0f) * (70.0f + (i % 4) * 25.0f));
    registry.AddComponent<ColorComp>(
        e, engine::renderer::Color(
               0.3f + 0.7f * ((i * 17) % 10) / 10.0f,
               0.3f + 0.7f * ((i * 31) % 10) / 10.0f,
               0.5f + 0.5f * ((i * 13) % 10) / 10.0f, 1.0f));
  }

  engine::core::Clock clock;
  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    const float dt = clock.Tick().GetSeconds();
    const float w = static_cast<float>(window->GetWidth());
    const float h = static_cast<float>(window->GetHeight());

    registry.GetView<Transform2D, Velocity2D>().Each(
        [&](engine::ecs::EntityId, Transform2D& t, Velocity2D& v) {
          t.x += v.vx * dt;
          t.y += v.vy * dt;
          if (t.x < 0.0f || t.x + t.size > w) {
            v.vx = -v.vx;
          }
          if (t.y < 0.0f || t.y + t.size > h) {
            v.vy = -v.vy;
          }
        });

    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::BeginBatch(w, h);
    registry.GetView<Transform2D, ColorComp>().Each(
        [](engine::ecs::EntityId, const Transform2D& t, const ColorComp& c) {
          engine::renderer::PrimitiveRenderer::DrawQuad(t.x, t.y, t.size,
                                                        t.size, c.color);
        });
    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "ECS demo completed (" << frame << " frames, "
            << registry.AliveCount() << " entities)." << std::endl;
  return 0;
}
