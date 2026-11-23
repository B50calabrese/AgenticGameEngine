# Agentic Game Engine

A lightweight, modern C++17 and OpenGL game engine architected for 2D games with multi-dimensional extensibility (2.5D/3D), first-class Lua scripting support, and built from the ground up to be cleanly consumable as a **git submodule**.

---

## Key Architectural Principles

* **Submodule-First Design**: The engine is designed to be embedded seamlessly into downstream game repositories. Public headers in `include/` are strictly decoupled from internal implementation in `src/`, preventing header pollution in consumer projects.
* **Modern CMake & Ergonomic Makefile**: Provides a namespaced CMake target (`engine::engine`) for consumers and a top-level `Makefile` for one-command developer convenience (`make build`, `make test`, `make demo`, `make format`).
* **2D-Focused with Multi-Dimensional Extensibility**: Optimized for 2D tilemaps, sprites, and batch rendering, while structuring math primitives, transform hierarchies, and cameras to naturally scale into 2.5D or 3D.
* **Strict Google C++ Standards**: Codebase strictly conforms to the Google C++ Style Guide, modern RAII, and memory safety practices.
* **Agent-Collaborative Development**: Governed by specialized AI subagents (`orchestrator`, `systems`, `coding`, `optimizer`, `docs`, `analyst`, `researcher`, `hr`) with formal Architecture Decision Records ([docs/adr/](docs/adr/)).

---

## Prerequisites

* **Compiler**: C++17 compatible compiler (GCC 9+, Clang 10+, MSVC 2019+)
* **Build System**: CMake 3.16+ and GNU Make
* **System Packages (Linux)**:
  ```bash
  sudo apt update && sudo apt install -y build-essential cmake libglfw3-dev libgl-dev
  ```
  *(Note: If GLFW is not installed on the system, CMake will automatically download and build it via `FetchContent` as a fallback).*

---

## Quick Start (Standalone Engine Development)

The top-level `Makefile` provides immediate commands for building and testing:

```bash
# Build the engine static library and sandbox demo
make build

# Run Google Test unit test suite
make test

# Launch the sandbox consumer demo
make demo

# Run Google C++ clang-format across all source files
make format

# Clean build artifacts
make clean
```

---

## Consuming the Engine as a Git Submodule

Downstream game developers can embed this engine into any GitHub repository in three simple steps:

### 1. Add Submodule
From your game repository root:
```bash
git submodule add https://github.com/B50calabrese/AgenticGameEngine.git third_party/engine
```

### 2. Configure Your Game's `CMakeLists.txt`
```cmake
cmake_minimum_required(VERSION 3.16)
project(MyGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the engine submodule
add_subdirectory(third_party/engine)

# Link your game to engine::engine
add_executable(MyGame src/main.cpp)
target_link_libraries(MyGame PRIVATE engine::engine)
```

### 3. Write Your Game Entrypoint (`src/main.cpp`)
```cpp
#include <iostream>
#include <engine/engine.h>

int main() {
  engine::core::Log::Initialize();
  ENGINE_LOG_INFO("Hello from MyGame! Engine Version: " + engine::GetVersionString());
  return 0;
}
```

For full integration details and advanced options, see the **[Submodule Consumer Guide](docs/submodule_guide.md)**.

---

## Project Structure

```text
.
├── include/engine/        # Public API headers (exposed to submodule consumers)
│   ├── core/              # Base types, logging, version, assertions
│   └── engine.h           # Umbrella header for consumers
├── src/engine/            # Internal engine implementation (private)
│   └── core/              # Core implementation files
├── tests/                 # Unit test suite using Google Test
├── demos/                 # Runnable demo applications and sandboxes
├── docs/
│   ├── adr/               # Architecture Decision Records (ADRs)
│   └── submodule_guide.md # Downstream consumer integration guide
├── .agents/
│   ├── agents/            # Subagent specifications (orchestrator, systems, coding, etc.)
│   └── skills/            # Procedural skills (task breakdown, style guide, etc.)
├── AGENTS.md              # Project mandates, coding rules, and agent roster
├── TASKS.md               # Milestone tracking, active task board, session handoff logs
├── CMakeLists.txt         # Submodule-friendly root CMake build file
└── Makefile               # Ergonomic developer commands
```

---

## Architecture Decision Records (ADRs)

Key technical choices are formally recorded and tracked in [docs/adr/](docs/adr/):
* **[ADR-0001](docs/adr/0001-record-architecture-decisions.md)**: Record Architecture Decisions
* **[ADR-0002](docs/adr/0002-submodule-friendly-cmake-and-makefile.md)**: Submodule-Friendly Modern CMake & Makefile Architecture

---

## AI Collaboration Layer

This repository is optimized for autonomous and collaborative agent development. See **[AGENTS.md](AGENTS.md)** for persona boundaries, operational rules, and contribution guidelines.
