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
| `INFRA-03` | Onboard `analyst` and `researcher` agents with updated mandates | HR | `DONE` | `INFRA-02` |
| `BUILD-01` | Scaffold clean directory tree and top-level Makefile | Systems | `IN_PROGRESS` | `INFRA-03` |
| `BUILD-02` | Configure submodule-friendly root CMakeLists.txt | Systems / Coding | `TODO` | `BUILD-01` |

*Status definitions: `TODO`, `IN_PROGRESS`, `REVIEW`, `DONE`, `BLOCKED`.*

---

## 3. Session Handoff Log

Use this section to record progress at the end of each session so future sessions resume with zero ambiguity.

### Session: 2026-11-23 (Session 2 - Agent Onboarding & Task 1 Inception)
* **Goal**: Onboard `analyst` and `researcher` agents; begin Milestone 1 (Task 1: Core Foundation & Build System).
* **Completed**:
  * Onboarded `analyst` (.agents/agents/analyst.md) with mandate to dissect problems and proactively ask user questions.
  * Onboarded `researcher` (.agents/agents/researcher.md) with mandate to survey industry prior art and critically evaluate reference repo without treating it as a gold standard.
  * Updated [AGENTS.md](AGENTS.md) rules and roster.
* **In Progress**: Dissecting Task 1 (`BUILD-01` & `BUILD-02`) and researching prior art for submodule-friendly build layouts.
* **Immediate Next Steps**:
  1. Have `analyst` dissect the requirements for Task 1 and present any clarifying questions.
  2. Have `researcher` compare prior art for CMake submodule patterns and top-level Makefile ergonomics.
  3. Initialize `docs/adr/` with ADR-0001.
