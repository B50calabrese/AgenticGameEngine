# 0003: Abstract Window Interface and GLFW Backend

* **Status**: Accepted
* **Date**: 2026-11-23
* **Author(s)**: Systems Architect & Coding Specialist
* **Deciders**: Antonio Calabrese, Orchestrator

## Context and Problem Statement

The engine needs windowing support to open display windows, receive OS events, and manage the OpenGL rendering context.

In the reference engine ([B50calabrese/GameEngine](https://github.com/B50calabrese/GameEngine)), the `Window` class was a concrete class that directly exposed a raw `GLFWwindow*` handle (`native_handle()`) in its public header. This introduced hard coupling to GLFW and leaked internal pointer types into downstream game logic.

## Considered Options

* **Option 1**: Concrete `Window` class directly wrapping GLFW in public headers (Reference implementation).
* **Option 2**: Abstract `Window` interface with static factory `Window::Create()` and backend implementations hidden in `src/engine/platform/` (Modern engine pattern).

## Decision Outcome

Chosen option: **Option 2 (Abstract Window Interface)**, because:
1. Public headers remain clean: downstream submodule consumers include `engine/platform/window.h` with zero GLFW or X11/Win32 header dependencies.
2. Cross-platform flexibility: alternative backends (SDL2, Win32, or headless mock for testing) can be introduced without altering existing game code or public APIs.
3. RAII compliance: window destruction and GLFW reference counting are completely encapsulated in `GlfwWindow`.

### Consequences

* **Positive**:
  * Downstream games are protected from platform-specific header pollution.
  * Facilitates headless automated testing.
* **Negative**:
  * One level of virtual dispatch (`OnUpdate()`, `GetWidth()`, etc.) per frame tick (negligible performance impact for window-level operations).
