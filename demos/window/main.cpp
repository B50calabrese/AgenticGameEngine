#include <chrono>
#include <iostream>
#include <thread>

#include "engine/engine.h"
#include "engine/platform/window.h"

int main(int argc, char** argv) {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("========================================");
  ENGINE_LOG_INFO("   Agentic Game Engine - Window Demo    ");
  ENGINE_LOG_INFO("   Engine Version: " + engine::GetVersionString());
  ENGINE_LOG_INFO("========================================");

  // Check if automated frame limit is passed
  int max_frames = 0;  // 0 means run until window is closed
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--frames" && i + 1 < argc) {
      max_frames = std::stoi(argv[++i]);
    }
  }

  engine::platform::WindowProps props;
  props.title = "Agentic Game Engine - Window Demo";
  props.width = 960;
  props.height = 540;
  props.vsync = true;

  auto window = engine::platform::Window::Create(props);
  if (!window) {
    ENGINE_LOG_ERROR("Could not create platform window.");
    ENGINE_LOG_WARN("If running in headless environment, ensure X11/Xvfb and 'libglfw3-dev' are available.");
    return 1;
  }

  ENGINE_LOG_INFO("Window created successfully!");
  if (max_frames > 0) {
    ENGINE_LOG_INFO("Running for " + std::to_string(max_frames) + " frames...");
  } else {
    ENGINE_LOG_INFO("Entering main window loop (close window to exit)...");
  }

  int frame_count = 0;
  while (!window->ShouldClose()) {
    window->OnUpdate();
    frame_count++;

    if (max_frames > 0 && frame_count >= max_frames) {
      ENGINE_LOG_INFO("Reached target frame limit: " + std::to_string(max_frames));
      break;
    }

    // Small sleep to avoid eating 100% CPU when no rendering pipeline is active
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }

  ENGINE_LOG_INFO("Window loop exited cleanly after " + std::to_string(frame_count) + " frames.");
  return 0;
}
