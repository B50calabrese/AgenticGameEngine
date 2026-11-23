#include <iostream>

#include "engine/engine.h"

int main() {
  std::cout << "========================================" << std::endl;
  std::cout << "  Agentic Game Engine - Sandbox Demo    " << std::endl;
  std::cout << "  Engine Version: " << engine::GetVersionString() << std::endl;
  std::cout << "========================================" << std::endl;

  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("Engine initialized successfully in sandbox demo.");
  ENGINE_LOG_INFO("Submodule consumer test complete.");

  return 0;
}
