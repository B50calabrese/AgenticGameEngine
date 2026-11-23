# Architecture Overview

This page describes the high-level architecture, subsystem topology, and layering of the **Agentic Game Engine**.

---

## 1. Design Philosophy

1. **Submodule-First (Consumer Isolation)**:
   The engine is architected to be embedded as a git submodule inside downstream game repositories. The public API lives exclusively under `include/engine/` and never exposes private internal headers, vendor headers, or low-level graphics handles.
2. **2D Focus with Multi-Dimensional Generalization**:
   The engine's initial focus is fast 2D gameplay (sprites, batching, 2D collisions, tilemaps), but foundational primitives (math, transforms, cameras, render queues) are architected to support 2.5D and 3D without breaking existing APIs.
3. **Data-Oriented & Cache-Friendly**:
   Hot loops (ECS iteration, physics integration, render batch generation) operate over contiguous memory buffers. No heap allocations occur inside the frame tick.

---

## 2. Layered Architecture

The engine is structured into five distinct layers. Dependencies flow strictly **downward**; circular dependencies between layers or subsystems are prohibited.

```text
┌───────────────────────────────────────────────────────────┐
│                    Downstream Game                        │
│            (Consumes engine::engine alias)                │
└─────────────────────────────┬─────────────────────────────┘
                              │
┌─────────────────────────────▼─────────────────────────────┐
│                   Application & Scene                     │
│               (Game loop, scene hierarchy)                │
└─────────────────────────────┬─────────────────────────────┘
                              │
┌─────────────────────────────▼─────────────────────────────┐
│                 Engine Subsystems Layer                   │
│   ┌──────────────┐  ┌──────────────┐  ┌───────────────┐   │
│   │     ECS      │  │   Renderer   │  │    Physics    │   │
│   │ (Registry,   │  │ (Batching,   │  │  (Collision,  │   │
│   │ Components)  │  │ RenderQueue) │  │   Broadphase) │   │
│   └──────────────┘  └──────────────┘  └───────────────┘   │
│   ┌──────────────┐  ┌──────────────┐                      │
│   │    Input     │  │  Scripting   │                      │
│   │  (Mapping)   │  │    (Lua)     │                      │
│   └──────────────┘  └──────────────┘                      │
└─────────────────────────────┬─────────────────────────────┘
                              │
┌─────────────────────────────▼─────────────────────────────┐
│                   Platform Abstraction                    │
│      (GLFW Windowing, OS events, OpenGL context loader)   │
└─────────────────────────────┬─────────────────────────────┘
                              │
┌─────────────────────────────▼─────────────────────────────┐
│                       Core Layer                          │
│     (Math Primitives, Memory, Assertions, Logging, Base)  │
└───────────────────────────────────────────────────────────┘
```

---

## 3. Subsystem Communication Rules

* **Queue-First Rendering**: Game systems never issue raw OpenGL commands. Systems generate draw commands and submit them to `RenderQueue`, which sorts by shader, texture, and Z-layer before flushing.
* **Snapshot ECS Iteration**: Systems iterate over snapshots or safe views. Entities scheduled for destruction during iteration are placed in a deferred destruction list and purged at the end of the frame tick.
* **Decoupled Scripting**: The Lua scripting subsystem interfaces via standard event hooks (`OnInit`, `OnUpdate`, `OnDestroy`) and interacts with entities through clean C++ bindings without direct access to raw engine storage pointers.
