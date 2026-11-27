# Task Tracking & Session Board

This document tracks all project milestones, active epics, task states, and cross-session handoff logs. Every agent turn that plans, starts, or completes tasks must update this file.

---

## 1. Project Milestones

- [x] **Milestone 0: Agentic Infrastructure & Foundation**
  - [x] Configure root rules and coding guidelines (`AGENTS.md`)
  - [x] Set up operational subagents (`orchestrator`, `hr`, `systems`, `coding`, `optimizer`, `docs`, `analyst`, `researcher`)
  - [x] Set up operational skills (`agent-hiring`, `task-breakdown`, `session-tracking`, `google-cpp-style`, `code-optimization`, `engine-documentation`)
  - [x] Establish ADR directory and initial template
  - [x] Initialize repository structure (`include/`, `src/`, `demos/`, `tests/`, `Makefile`, `CMakeLists.txt`)
- [x] **Milestone 1: Core Foundation & Build System**
  - [x] Submodule-friendly CMake configuration with top-level `Makefile` (`BUILD-01`, `BUILD-02`)
  - [x] Core versioning, logging, assertion macros, and Google Test smoke suite
  - [x] Platform abstraction layer (windowing & OpenGL context via GLFW) (`PLAT-01`)
  - [x] 2D rendering primitives, hardware abstraction layer (HAL), and interactive demo (`REND-01`)
  - [x] Core math primitives (Vector2, Vector3, Vector4, Matrix4, Transform) (`CORE-01`)
- [ ] **Milestone 2: Entity Component System (ECS)**
  - [ ] Data-oriented contiguous component storage (`ECS-01`)
  - [ ] Entity manager and recycled entity IDs (`ECS-02`)
  - [ ] Registry coordinator with view iteration (`ECS-03`)
- [ ] **Milestone 3: 2D/2.5D Batch Renderer**
  - [ ] Shader abstraction and OpenGL pipeline
  - [ ] `RenderQueue` with Z-sorting and automatic texture batching
  - [ ] Camera abstraction (2D orthographic with extensible 3D view projections)
- [ ] **Milestone 4: Scripting & Extensibility**
  - [ ] Lua state integration and C++ bindings
  - [ ] Script component and lifecycle hooks
- [ ] **Milestone 5: Demos & Consumer Game Sample**
  - [ ] Demo standalone game demonstrating submodule consumption

---

## 2. Active Task Board

| Task ID | Task Description | Owner | Status | Dependencies |
| :--- | :--- | :--- | :--- | :--- |
| `INFRA-01` | Equip workspace with agent suite and system instructions | Orchestrator | `DONE` | None |
| `INFRA-02` | Create operational skills (hiring, tracking, breakdown, C++) | HR / Docs | `DONE` | `INFRA-01` |
| `INFRA-03` | Onboard `analyst` and `researcher` agents with updated mandates | HR | `DONE` | `INFRA-02` |
| `BUILD-01` | Scaffold clean directory tree and top-level Makefile | Systems | `DONE` | `INFRA-03` |
| `BUILD-02` | Configure submodule-friendly root CMakeLists.txt | Systems / Coding | `DONE` | `BUILD-01` |
| `WIKI-01` | Formalize living Wiki architecture & continuous documentation invariant | Docs / Orchestrator | `DONE` | `BUILD-02` |
| `PLAT-01` | Implement abstract window interface and GLFW backend | Systems / Coding | `DONE` | `BUILD-02` |
| `REND-01` | Implement 2D rendering primitives, HAL (buffers, VAO, shader), and primitives demo | Systems / Coding | `DONE` | `PLAT-01` |
| `CORE-01` | Implement core math primitives (Vector2, Vector3, Matrix4, Transform) | Systems / Coding | `DONE` | `BUILD-02` |
| `INP-01`  | Implement polling Input system (`KeyCode`, `MouseButton`) and `Clock`/`Timestep` | Systems / Coding | `DONE` | `PLAT-01` |
| `GAME-01` | Build interactive Breakout demo game (`demos/breakout`) | Coding | `DONE` | `INP-01`, `REND-01` |
| `GAME-02` | Build interactive 2D Platformer demo game (`demos/platformer`) | Coding | `DONE` | `INP-01`, `REND-01` |
| `ECS-01`  | Implement data-oriented contiguous component storage (SparseSet) | Systems / Coding | `TODO` | `CORE-01` |

*Status definitions: `TODO`, `IN_PROGRESS`, `REVIEW`, `DONE`, `BLOCKED`.*

---

## 3. Session Handoff Log

Use this section to record progress at the end of each session so future sessions resume with zero ambiguity.

### Session: 2026-11-27 (Session 6 - Input System, Frame Clock & Playable Demo Games)
* **Goal**: Implement `INP-01` (keyboard/mouse polling + `Clock`/`Timestep`) and build two playable demo games (`GAME-01` Breakout and `GAME-02` 2D Platformer) to validate engine ergonomics prior to ECS.
* **Completed**:
  * Implemented `include/engine/core/timestep.h` and `include/engine/core/clock.h` (`src/engine/core/clock.cpp`).
  * Implemented `include/engine/input/` (`key_codes.h`, `mouse_button_codes.h`, `input.h`) and `src/engine/input/input.cpp`, with automatic active-window binding in `GlfwWindow`.
  * Added `tests/input_test.cpp` (all 35/35 tests passing across engine).
  * Authored living wiki page [docs/subsystems/input.md](docs/subsystems/input.md) and ADR [docs/adr/0006-polling-input-and-frame-timestep.md](docs/adr/0006-polling-input-and-frame-timestep.md).
  * Built `demos/breakout/main.cpp` (`make breakout`) and `demos/platformer/main.cpp` (`make platformer`), both supporting `--frames N` headless validation.
* **Immediate Next Steps**:
  1. User review of local commit before pushing to `main`.
  2. Proceed to **Milestone 2 (ECS)** (`ECS-01`) when ready, using Breakout/Platformer as refactoring testbeds.

### Session: 2026-11-24 (Session 5 - Core Math Primitives & Milestone 1 Completion)
* **Goal**: Implement decoupled, vendor-isolated linear algebra structures (`Vector2`, `Vector3`, `Vector4`, `Matrix4`, `Transform`, `MathUtils`) and complete Milestone 1.
* **Completed**:
  * Implemented `include/engine/math/`: `math_utils.h`, `vector2.h`, `vector3.h`, `vector4.h`, `matrix4.h`, and `transform.h`.
  * Implemented `src/engine/math/`: `matrix4.cpp` (column-major, matrix multiplication, transform point/dir, 4x4 cofactor inversion, determinant, Orthographic, Perspective, LookAt) and `transform.cpp`.
  * Integrated math types with `PrimitiveRenderer` (`BeginBatch(Matrix4)`, `DrawQuad(Vector2, Vector2, Color)`, etc.) and updated primitives demo.
  * Added unit test suite `tests/math_test.cpp` with 16 comprehensive tests (all 31/31 tests passing across engine).
  * Authored living wiki page [docs/subsystems/math.md](docs/subsystems/math.md) and ADR [docs/adr/0005-linear-algebra-and-transform-primitives.md](docs/adr/0005-linear-algebra-and-transform-primitives.md).
  * Completed **Milestone 1 (Core Foundation & Build System)** in full.
* **Immediate Next Steps**:
  1. Commit and push math subsystem to GitHub.
  2. Advance to **Milestone 2 (Entity Component System)** with `ECS-01` (SparseSet component storage).

### Session: 2026-11-24 (Session 4 - 2D Rendering Primitives & Hardware Abstraction Layer)
* **Goal**: Design and build hardware-accelerated 2D rendering primitives (quads, triangles, lines, circles, rect outlines), low-level HAL (VertexBuffer, IndexBuffer, VertexArray, Shader, RenderCommand), and an interactive showcase demo.
* **Completed**:
  * Vendored GLAD OpenGL loader under `thirdparty/glad` with compiler warning suppression (`-w`).
  * Implemented public clean headers under `include/engine/renderer/`: `color.h`, `buffer.h`, `vertex_array.h`, `shader.h`, `render_command.h`, and `primitive_renderer.h`.
  * Implemented OpenGL backend in `src/engine/renderer/`: `opengl_buffer.cpp`, `opengl_vertex_array.cpp`, `opengl_shader.cpp`, `render_command.cpp`, and `primitive_renderer.cpp`.
  * Built unified data-oriented 2D batching engine supporting colored quads, rotated quads, triangles, arbitrary thickness lines, and circles with zero per-frame heap allocations.
  * Added unit test suite in `tests/renderer_test.cpp` (all 15/15 tests passing across engine).
  * Built rich showcase demo `demos/primitives/main.cpp` with `make primitives-demo` (runs on hardware NVIDIA context with headless `--frames` support).
  * Authored living wiki page [docs/subsystems/renderer.md](docs/subsystems/renderer.md) and ADR [docs/adr/0004-rendering-primitives-and-hardware-abstraction.md](docs/adr/0004-rendering-primitives-and-hardware-abstraction.md).
* **Immediate Next Steps**:
  1. Commit and push rendering primitives subsystem to GitHub.
  2. Proceed to `CORE-01` (Core math primitives: Vector2, Vector3, Matrix4).

### Session: 2026-11-23 (Session 3 - Living Wiki Architecture & Documentation Invariant)
* **Goal**: Establish repository documentation as an active, living wiki and enforce documentation invariant across all agents.
* **Completed**:
  * Created structured Wiki portal at [docs/README.md](docs/README.md) with sections for Architecture, Subsystems, Guides, Standards, and ADRs.
  * Authored [docs/architecture/overview.md](docs/architecture/overview.md) detailing layer boundaries and data flow rules.
  * Authored [docs/subsystems/core.md](docs/subsystems/core.md) as the first living subsystem wiki page.
  * Authored [docs/standards/wiki_maintenance.md](docs/standards/wiki_maintenance.md) and [docs/standards/cpp_guidelines.md](docs/standards/cpp_guidelines.md).
  * Promoted `docs` agent to **Chief Wiki Steward** in [.agents/agents/docs.md](.agents/agents/docs.md).
  * Enforced **Wiki Gatekeeper Protocol** in [.agents/agents/orchestrator.md](.agents/agents/orchestrator.md) and [AGENTS.md](AGENTS.md): No major task is marked `DONE` without updating `docs/`.
  * Updated [engine-documentation skill](.agents/skills/engine-documentation/SKILL.md) with living wiki template and SOP.
* **Immediate Next Steps**:
  1. Commit living wiki architecture to git.
  2. Dissect and implement `CORE-01` (Math primitives: Vector2, Vector3, Matrix4) following the new wiki-first invariant.
