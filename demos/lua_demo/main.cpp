#include <cstdlib>
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

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Lua Scripting Demo";
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

  std::string err;
  const char* script = R"(
    engine.log("Lua demo script initialized!")
    t = 0
    function on_render()
      t = t + 0.03
      local offset = math.sin(t) * 120
      engine.draw_quad(180 + offset, 220, 120, 120, 0.2, 0.7, 0.9, 1.0)
      engine.draw_circle(680 - offset, 280, 60, 0.95, 0.4, 0.3, 1.0)
    end
  )";
  if (!lua.ExecuteString(script, &err)) {
    std::cerr << "Lua error: " << err << std::endl;
    return 1;
  }

  int frame = 0;
  while (!window->ShouldClose()) {
    if (max_frames >= 0 && frame >= max_frames) {
      break;
    }

    engine::renderer::RenderCommand::SetClearColor(
        engine::renderer::Color(0.08f, 0.10f, 0.14f, 1.0f));
    engine::renderer::RenderCommand::Clear();

    engine::renderer::PrimitiveRenderer::BeginBatch(
        static_cast<float>(window->GetWidth()),
        static_cast<float>(window->GetHeight()));

    if (!lua.CallFunction("on_render", &err)) {
      std::cerr << "Lua runtime error: " << err << std::endl;
      break;
    }

    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  std::cout << "Lua demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
