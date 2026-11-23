# Agentic Game Engine - Core Mandates & Rules

Welcome to the **Agentic Game Engine** project. This repository is built using an agent-collaborative development model. All agents and human collaborators operating in this workspace must adhere strictly to the directives below.

---

## 1. Project Identity & Architecture Goals

* **Scope**: A modern, lightweight game engine built with **C++** and **OpenGL**, focused primarily on **2D** but architected to support dimensions beyond 2D (e.g., 2.5D, 3D math primitives, general transform hierarchies, and multi-dimensional camera/render abstractions).
* **Consumer-First (Git Submodule)**: The engine is designed to be embedded as a git submodule in downstream game projects. Public headers in `include/` must remain clean, modular, and free of unnecessary internal implementation leakage.
* **Build Usability**: Simple and rapid developer iteration. High-level `Makefile` targets wrap the build system (e.g. `make build`, `make test`, `make demo`) to ensure low friction for developers and downstream consumers.
* **Scripting**: First-class **Lua** scripting support for rapid gameplay prototyping and configuration.
* **Cross-Platform Readiness**: Designed with platform abstraction layers so that Linux, Windows, and macOS targets can be supported seamlessly.

---

## 2. Reference Engine & Permitted Access

* **Reference Repository**: [B50calabrese/GameEngine](https://github.com/B50calabrese/GameEngine).
* **Reference Sanction**: Agents are **explicitly permitted to use `curl`** to inspect the reference repository, its raw GitHub content, and the GitHub API for code patterns, architectural references, and documentation.
* **Not a Gold Standard**: The reference repository is a historical baseline and architectural starting point, **never a gold standard**. Critically evaluate its design, actively identify shortcomings or anti-patterns, and compare against modern industry standards.

---

## 3. Strict Google C++ Coding Standards

All C++ code submitted to this repository must follow the **Google C++ Style Guide**:

1. **Naming Conventions**:
   * Types, Classes, Structs, Enums: `PascalCase` (e.g., `RenderQueue`, `TransformComponent`).
   * Functions and Methods: `PascalCase` (e.g., `Initialize()`, `Update()`).
   * Variables, Parameters, Namespaces: `snake_case` (e.g., `delta_time`, `entity_id`).
   * Private / Protected Class Members: Trailing underscore (e.g., `registry_`, `is_running_`).
   * Constants and Enumerators: `kPascalCase` (e.g., `kMaxBatchSize`, `kDefaultWidth`).
   * Macros: `ALL_CAPS` (keep macros to an absolute minimum).
2. **Directory & File Structure**:
   * Directory names: Purely lowercase alphabetical characters (e.g., `ecs`, `renderer`, `input`, `core`). Avoid hyphens or underscores in directory names.
   * File names: `snake_case` (e.g., `batch_renderer.h`, `transform_component.cpp`).
   * Public API: Placed under `include/engine/<subsystem>/`.
   * Internal Implementation: Placed under `src/engine/<subsystem>/`.
3. **Control Structures & Bracing**:
   * Always use curly braces for control flow statements (`if`, `for`, `while`), even for single-line bodies.
4. **Memory & Pointers**:
   * Follow RAII strictly.
   * Avoid raw owning pointers; use `std::unique_ptr` and `std::shared_ptr` intentionally.
   * Google output parameter convention: Use non-const pointers for output parameters (e.g., `void GetBounds(Rect* out_bounds)`), const references for read-only input objects.
5. **Namespaces**:
   * All engine code lives inside the `engine` namespace (or nested subsystem namespaces like `engine::ecs`, `engine::gfx`).
   * `using namespace` directives in header files are **strictly forbidden**.

---

## 4. Agent Suite & Division of Labor

The workspace is maintained by a specialized, hyper-focused agent suite:

| Agent | Responsibility | Primary Boundaries |
| :--- | :--- | :--- |
| **`orchestrator`** | Project management, task planning, subagent dispatching, milestone verification. | Does not write engine code directly; delegates to specialists. |
| **`analyst`** | Problem dissection, requirements extraction, edge-case probing, asking clarifying questions. | Does not implement code or conduct broad external research; focuses on problem clarity. |
| **`researcher`** | Prior art investigation, comparative library/pattern analysis, reference codebase inspection. | Does not treat reference as gold standard; does not make final architectural decisions. |
| **`hr`** | Onboarding new agents, drafting agent specifications, maintaining skills. | Requires human approval before registering any new agent. |
| **`systems`** | Subsystem architecture, cross-module contracts, public API stability. | Ensures subsystems stay decoupled, modular, and submodule-friendly. |
| **`coding`** | C++ implementation, unit testing with Google Test, formatting. | Implements code strictly according to Google C++ and system contracts. |
| **`optimizer`** | Performance engineering, cache locality, DOD, batch efficiency, profiling. | Focuses on throughput, memory layout, and cross-machine performance. |
| **`docs`** | Architectural Decision Records (ADRs), Doxygen comments, submodule guides. | Maintains clear, up-to-date documentation for human and agent developers. |

---

## 5. Workflow Rules: Tasks, Sessions & Decisions

1. **Task-Driven Work**:
   * Every non-trivial operation must be tracked in [TASKS.md](TASKS.md).
   * Tasks must follow the Work Breakdown Structure: **Contract/Headers First $\rightarrow$ Implementation $\rightarrow$ Tests & Validation**.
2. **Cross-Session Continuity**:
   * At the beginning of each session, read [TASKS.md](TASKS.md) and recent ADRs in `docs/adr/` to restore state.
   * At the end of each session, update the **Session Handoff Log** in [TASKS.md](TASKS.md).
3. **Architectural Decisions**:
   * Major architectural decisions (subsystem boundaries, library additions, API overhauls) must be documented as an ADR in `docs/adr/`.
4. **Agent Hiring Protocol**:
   * Any new agent persona must be processed through the `hr` agent and requires explicit human approval before creation.
