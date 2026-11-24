# ADR 0004: 2D Rendering Primitives & Hardware Abstraction Layer

* **Status**: Accepted
* **Date**: 2026-11-24
* **Deciders**: Systems Architect, Lead Developer, User

## Context and Problem Statement

Before introducing higher-level math structures or full scene graphs, the engine requires a reliable, hardware-accelerated rendering foundation to draw 2D geometry (quads, triangles, lines, circles, and bounding boxes) directly onto the screen.

In the historical reference repository (`B50calabrese/GameEngine`), primitive rendering suffered from multiple anti-patterns:
1. An oversized monolithic 300+ line uber-shader string embedded into source code with 12 vertex attributes per vertex (including unused booleans and shape types), severely bloating the vertex stride.
2. Direct leakage of vendor headers (`glm`, `glad`) into public engine headers.
3. No decoupled Hardware Abstraction Layer (HAL) for GPU buffers and shader programs.
4. Hardcoded static singleton state without RAII cleanup.

We need a clean, modern, data-oriented rendering architecture conforming to Google C++ Style and strict submodule header encapsulation.

## Decision Drivers

* **Performance & Data-Oriented Design**: Minimize vertex stride (only position and color needed for geometric primitives) and avoid per-frame allocations during batching.
* **Header Encapsulation**: Downstream consumers in `include/engine/renderer/` must never see raw OpenGL tokens, GLAD types, or GLFW headers.
* **Unified Batching**: Quads, rotated quads, triangles, lines, and circles share an indexed vertex batch wherever possible, minimizing GPU state changes and draw calls.
* **Immediate Visual Feedback**: Provide an interactive demo (`primitives_demo`) verifying windowing, context initialization, shader compilation, and GPU rasterization.

## Considered Options

1. **Option 1: Monolithic Shader & Immediate Mode**: Emulate reference repo with massive uber-shader and immediate vertex dispatch. (Rejected: inefficient memory layout, high GPU overhead).
2. **Option 2: High-Level Scene Graph First**: Implement transforms, cameras, and scenes before drawing basic shapes. (Rejected: delays visual validation and creates chicken-and-egg dependency).
3. **Option 3: Layered HAL + Batched 2D Primitive Renderer (Selected)**:
   * **Low-Level HAL**: `VertexBuffer`, `IndexBuffer`, `BufferLayout`, `VertexArray`, `Shader`, and `RenderCommand`.
   * **High-Level 2D Batching**: `PrimitiveRenderer` with `Color` POD, dynamic vertex buffers, static index topology, and built-in 2D orthographic projection.

## Decision Outcome

Option 3 was chosen and implemented.

### Positive Consequences
* **Extreme Efficiency**: 1 draw call per frame can render thousands of quads, triangles, circles, and thick lines.
* **Zero Leakage**: All OpenGL specifics (`glad.h`, `GL_*` constants) are hidden inside `src/engine/renderer/`.
* **Clean RAII**: GPU buffers, vertex arrays, and shader programs clean up their resources in destructors.
* **Decoupled Math**: Primitives operate with basic float inputs and internal projection calculations, cleanly unblocking development before full vector/matrix math libraries are formalized.

### Negative Consequences / Trade-Offs
* Third-party GLAD loader is vendored in `thirdparty/glad/` (isolated from public headers and silenced from warnings).
