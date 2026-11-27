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
  props.title = "Agentic Game Engine - Audio Demo";
  props.width = 960;
  props.height = 540;
  auto window = engine::platform::Window::Create(props);
  if (!window) {
    std::cerr << "Failed to create window." << std::endl;
    return 1;
  }

  engine::audio::AudioEngine::Initialize();
  auto click_sfx = engine::audio::AudioEngine::LoadSound("sfx/click.wav");

  engine::renderer::PrimitiveRenderer::Initialize();
  engine::renderer::RenderCommand::SetViewport(0, 0, window->GetWidth(),
                                               window->GetHeight());

  auto font = engine::renderer::Font::Load(
      "thirdparty/fonts/Quicksand-Regular.ttf", 22);

  int sfx_count = 0;
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

    ui.Label("Audio Engine Demo", {80.0f, 420.0f}, font,
             engine::renderer::Color::White());
    ui.Label("SFX Played: " + std::to_string(sfx_count), {80.0f, 375.0f}, font,
             engine::renderer::Color::Yellow());
    ui.Label(std::string("Music: ") +
                 (engine::audio::AudioEngine::IsMusicPlaying() ? "Playing"
                                                               : "Stopped"),
             {80.0f, 340.0f}, font, engine::renderer::Color::Cyan());

    if (ui.Button("sfx", {80.0f, 260.0f, 240.0f, 44.0f}, "Play SFX", font)) {
      engine::audio::AudioEngine::PlaySfx(click_sfx);
      ++sfx_count;
    }
    if (ui.Button("music", {80.0f, 200.0f, 240.0f, 44.0f}, "Toggle Music",
                  font)) {
      if (engine::audio::AudioEngine::IsMusicPlaying()) {
        engine::audio::AudioEngine::StopMusic();
      } else {
        engine::audio::AudioEngine::PlayMusic("music/ambient.ogg");
      }
    }
    if (ui.Button("vol", {80.0f, 140.0f, 240.0f, 44.0f}, "Cycle Volume",
                  font)) {
      float v = engine::audio::AudioEngine::GetMasterVolume() - 0.25f;
      if (v < 0.0f) {
        v = 1.0f;
      }
      engine::audio::AudioEngine::SetMasterVolume(v);
    }

    ui.EndFrame();
    engine::renderer::PrimitiveRenderer::EndBatch();
    window->OnUpdate();
    ++frame;
  }

  engine::renderer::PrimitiveRenderer::Shutdown();
  engine::audio::AudioEngine::Shutdown();
  std::cout << "Audio demo completed (" << frame << " frames)." << std::endl;
  return 0;
}
