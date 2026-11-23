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
- [ ] **Milestone 1: Core Foundation & Build System**
  - [x] Submodule-friendly CMake configuration with top-level `Makefile` (`BUILD-01`, `BUILD-02`)
  - [x] Core versioning, logging, assertion macros, and Google Test smoke suite
  - [ ] Platform abstraction layer (windowing & OpenGL context via GLFW)
  - [ ] Core math primitives (Vector2, Vector3, Matrix4) and general transform hierarchy
- [ ] **Milestone 2: Entity Component System (ECS)**
  - [ ] Data-oriented contiguous component storage
  - [ ] Entity manager and recycled entity IDs
  - [ ] Registry coordinator with view iteration
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
| `CORE-01` | Implement core math primitives (Vector2, Vector3, Matrix4) | Systems / Coding | `TODO` | `BUILD-02` |
| `PLAT-01` | Implement window and OpenGL context abstraction | Systems / Coding | `TODO` | `BUILD-02` |

*Status definitions: `TODO`, `IN_PROGRESS`, `REVIEW`, `DONE`, `BLOCKED`.*

---

## 3. Session Handoff Log

Use this section to record progress at the end of each session so future sessions resume with zero ambiguity.

### Session: 2026-11-23 (Session 2 - Task 1 Completion & Validation)
* **Goal**: Deliver Task 1 (`BUILD-01` & `BUILD-02`): Core foundation, submodule-friendly CMake, and top-level Makefile.
* **Completed**:
  * Scaffolded directory tree: `include/engine/core/`, `src/engine/core/`, `tests/`, `demos/`.
  * Created [Makefile](Makefile) with targets: `build`, `test`, `demo`, `format`, `clean`.
  * Created [CMakeLists.txt](CMakeLists.txt) with C++17, namespaced target `engine::engine`, and `Option A` FetchContent fallback.
  * Implemented core modules: `base.h`, `version.h`, `log.h`, `assert.h` and implementations.
  * Created [core_test.cpp](tests/core_test.cpp) (Google Test) and [demos/sandbox/main.cpp](demos/sandbox/main.cpp).
  * Validated build: `make test` passed 100% (3/3 tests) and `make demo` executed successfully.
  * Authored [docs/adr/0002-submodule-friendly-cmake-and-makefile.md](docs/adr/0002-submodule-friendly-cmake-and-makefile.md).
* **Immediate Next Steps**:
  1. Commit Task 1 files to git.
  2. Dissect and design `CORE-01` (2D/3D math primitives: Vector2, Vector3, Matrix4) and `PLAT-01` (Window & Context abstraction).
