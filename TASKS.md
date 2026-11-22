# Task Tracking & Session Board

This document tracks all project milestones, active epics, task states, and cross-session handoff logs. Every agent turn that plans, starts, or completes tasks must update this file.

---

## 1. Project Milestones

- [ ] **Milestone 0: Agentic Infrastructure & Foundation**
  - [x] Configure root rules and coding guidelines (`AGENTS.md`)
  - [x] Set up operational subagents (`orchestrator`, `hr`, `systems`, `coding`, `optimizer`, `docs`)
  - [x] Set up operational skills (`agent-hiring`, `task-breakdown`, `session-tracking`, `google-cpp-style`, `code-optimization`, `engine-documentation`)
  - [x] Establish ADR directory and initial template
  - [ ] Initialize repository structure (`include/`, `src/`, `demos/`, `tests/`, `Makefile`, `CMakeLists.txt`)
- [ ] **Milestone 1: Core Foundation & Build System**
  - [ ] Submodule-friendly CMake configuration with top-level `Makefile`
  - [ ] Platform abstraction layer (windowing & OpenGL context via GLFW)
  - [ ] Core logging, math primitives (Vector2, Vector3, Matrix4), and assertions
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
| `BUILD-01` | Scaffold clean directory tree and top-level Makefile | Systems | `TODO` | `INFRA-02` |
| `BUILD-02` | Configure submodule-friendly root CMakeLists.txt | Systems / Coding | `TODO` | `BUILD-01` |

*Status definitions: `TODO`, `IN_PROGRESS`, `REVIEW`, `DONE`, `BLOCKED`.*

---

## 3. Session Handoff Log

Use this section to record progress at the end of each session so future sessions resume with zero ambiguity.

### Session: 2026-11-22 (Session 1 - Infrastructure Setup)
* **Goal**: Equip repository with agent suite, operational skills, task board, and ADR framework.
* **Completed**:
  * Established [AGENTS.md](AGENTS.md) with Google C++ mandates, curl permissions for reference repo, and agent boundaries.
  * Configured full subagent suite under `.agents/agents/` (`orchestrator`, `hr`, `systems`, `coding`, `optimizer`, `docs`).
  * Configured operational skills under `.agents/skills/` (`agent-hiring`, `task-breakdown`, `session-tracking`, `google-cpp-style`, `code-optimization`, `engine-documentation`).
  * Initialized Architecture Decision Records under `docs/adr/` with template and ADR-0001.
  * Formatted `.clang-format` enforcing Google C++ Style.
* **In Progress**: Milestone 0 foundation complete; awaiting user review before initiating project scaffolding and code.
* **Immediate Next Steps**:
  1. User review of agent suite and skills.
  2. Proceed to Task `BUILD-01`: Scaffolding directory tree (`include/`, `src/`, `demos/`, `tests/`) and `Makefile`.
  3. Initialize `docs/adr/` with ADR-0001.
