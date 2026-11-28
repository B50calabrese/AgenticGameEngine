#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "engine/engine.h"

int main(int argc, char** argv) {
  int max_frames = -1;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--frames" && i + 1 < argc) {
      max_frames = std::atoi(argv[++i]);
    }
  }

  const std::string script_path = "/tmp/age_ecs_lua_demo.lua";
  {
    std::ofstream out(script_path);
    out << "x = 160\n"
        << "y = 220\n"
        << "t = 0\n"
        << "function on_create()\n"
        << "  engine.log('ECS Lua script loaded!')\n"
        << "end\n"
        << "function on_update()\n"
        << "  t = t + dt\n"
        << "  x = 420 + math.sin(t * 2.5) * 200\n"
        << "  engine.draw_quad(x, y, 100, 100, 0.3, 0.85, 0.5, 1.0)\n"
        << "end\n";
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - ECS + Lua Hot-Reload Demo";
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

  engine::scripting::LuaState lua;
  lua.RegisterEngineBindings();

  engine::ecs::Registry registry;
  auto e = registry.CreateEntity();
  registry.AddComponent<engine::ecs::TagComponent>(e, "LuaActor");
  registry.AddComponent<engine::ecs::ScriptComponent>(e, script_path);

  engine::core::Clock clock;
  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    const float dt = clock.Tick().GetSeconds();
    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::BeginBatch(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight()));

    engine::scripting::ScriptSystem::Update(registry, lua, dt);

    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "ECS+Lua demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
