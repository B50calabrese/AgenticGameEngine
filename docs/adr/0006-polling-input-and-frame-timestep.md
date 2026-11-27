# ADR 0006: Polling Input Architecture & Frame Timestep Clock

* **Status**: Accepted
* **Date**: 2026-11-25
* **Deciders**: Systems Architect, Lead Developer, User

## Context and Problem Statement

To support playable gameplay prototyping and interactive validation prior to ECS, the engine requires a mechanism for polling user keyboard and mouse input as well as tracking frame delta time (`delta_time`).

In the reference repository (`B50calabrese/GameEngine`), an `ActionManager` and `InputManager` were implemented with heavy dependencies on custom event buses, UI systems, and raw GLFW callbacks. Before building advanced action mapping or ECS-driven input dispatch, games need an ergonomic, zero-friction, static polling API (`Input::IsKeyPressed(KeyCode::Space)`).

## Decision Drivers

* **Zero-Boilerplate**: Game loops can immediately query keyboard and mouse state without wiring complex listeners or passing window handles to every entity.
* **Decoupled Platform**: Downstream game code only depends on `engine::input::KeyCode` and `engine::input::MouseButton`, never GLFW tokens (`GLFW_KEY_*`).
* **Frame-Rate Independence**: `Timestep` and `Clock` provide standard delta time in seconds and milliseconds using `std::chrono`.
* **Headless Safety**: Queries return safe defaults if no window is active or in unit test runners.

## Decision Outcome

1. Implemented `engine::input::Input` with automatic active window binding via `GlfwWindow`.
2. Implemented `engine::core::Timestep` and `engine::core::Clock`.
3. Validated with unit tests in `tests/input_test.cpp`.
