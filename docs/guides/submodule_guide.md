# Submodule Consumer Integration Guide

This guide details how to integrate the **Agentic Game Engine** into downstream GitHub game projects as a git submodule.

---

## 1. Adding the Submodule

In your game's repository, run:

```bash
git submodule add https://github.com/B50calabrese/AgenticGameEngine.git third_party/engine
git submodule update --init --recursive
```

This clones the engine into `third_party/engine` without checking in raw duplicate files.

---

## 2. CMake Integration

The engine is engineered specifically for clean `add_subdirectory()` consumption. When consumed as a submodule:
* Engine unit tests (`ENGINE_BUILD_TESTS`) and demos (`ENGINE_BUILD_DEMOS`) are **automatically disabled**.
* Only the clean public API in `include/` is exposed to your project. Internal headers in `src/` remain private.

### Sample `CMakeLists.txt` for your Game:
```cmake
cmake_minimum_required(VERSION 3.16)
project(SuperSpaceShooter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Embed Engine
add_subdirectory(third_party/engine)

# 2. Define Game Executable
add_executable(SuperSpaceShooter
    src/main.cpp
    src/player.cpp
)

# 3. Link against engine::engine alias
target_link_libraries(SuperSpaceShooter
    PRIVATE
        engine::engine
)
```

---

## 3. Minimal Game Loop Example

Here is a minimal, compilable `src/main.cpp` using the public engine API:

```cpp
#include <iostream>
#include <engine/engine.h>

int main() {
  // Initialize logging
  engine::core::Log::Initialize();
  engine::core::Log::SetLevel(engine::core::LogLevel::kInfo);

  ENGINE_LOG_INFO("Starting Game!");
  ENGINE_LOG_INFO("Running on Engine Version: " + engine::GetVersionString());

  // Game loop placeholder
  bool is_running = true;
  int frame_count = 0;

  while (is_running) {
    // Process input, update systems, render frame
    frame_count++;

    if (frame_count >= 10) {
      is_running = false;
    }
  }

  ENGINE_LOG_INFO("Game exited cleanly after " + std::to_string(frame_count) + " frames.");
  return 0;
}
```

---

## 4. Best Practices for Submodule Consumers

1. **Include Pattern**:
   Always include public headers using the namespaced prefix:
   ```cpp
   #include <engine/engine.h>
   #include <engine/core/log.h>
   #include <engine/core/version.h>
   ```
2. **Never Touch Engine Internals**:
   Do not include headers from `src/engine/`. If you need functionality not yet exposed, submit an issue or feature request to add a clean public interface under `include/engine/`.
3. **Updating the Engine**:
   To pull the latest engine updates into your game:
   ```bash
   cd third_party/engine
   git pull origin main
   cd ../..
   git add third_party/engine
   git commit -m "chore: bump engine submodule to latest"
   ```
