# 0002: Submodule-Friendly Modern CMake and Makefile Architecture

* **Status**: Accepted
* **Date**: 2026-11-23
* **Author(s)**: Systems Architect & Prior Art Researcher
* **Deciders**: Antonio Calabrese, Orchestrator

## Context and Problem Statement

The engine must be consumable as a git submodule in downstream game projects without leaking internal implementation details or causing include directory/library collisions. At the same time, local developers require rapid, low-friction iteration (`make build`, `make test`, `make demo`, `make format`).

The reference engine ([B50calabrese/GameEngine](https://github.com/B50calabrese/GameEngine)) exposed vendor headers publicly (`PUBLIC ${THIRD_PARTY_INCLUDE}`) and hardcoded static library paths (`glfw3.lib`), which creates major friction for downstream projects and portability.

## Considered Options

* **Option 1**: Monolithic CMakeLists with raw target names, globbed sources, and vendored precompiled binaries.
* **Option 2**: Pure `make` with manual compiler flags.
* **Option 3**: Target-based modern CMake (C++17) with `engine::engine` alias, strict `$<BUILD_INTERFACE>` / `$<INSTALL_INTERFACE>` encapsulation, dependency resolution via system packages with `FetchContent` fallback, wrapped by an ergonomic developer `Makefile`.

## Decision Outcome

Chosen option: **Option 3**, because:
1. `engine::engine` target namespace guarantees no collision with downstream consumer targets.
2. `BUILD_INTERFACE` exposes only `include/` to consumers; internal headers in `src/` remain private.
3. Tests and demos are automatically skipped when consumed as a submodule (`add_subdirectory`), eliminating unnecessary overhead for downstream consumers.
4. Top-level `Makefile` provides one-command convenience for developers on Linux/macOS.

### Consequences

* **Positive**:
  * Clean, seamless submodule embedding: `target_link_libraries(MyGame PRIVATE engine::engine)`.
  * Zero build artifact pollution for downstream projects.
  * Dependency fallback ensures successful builds on both bare machines and fully provisioned dev boxes.
* **Negative**:
  * Initial build on machines without GLFW/GTest takes extra seconds to fetch and compile dependencies via `FetchContent`.
