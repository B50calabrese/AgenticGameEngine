# ADR 0005: Linear Algebra & Spatial Transform Primitives

* **Status**: Accepted
* **Date**: 2026-11-24
* **Deciders**: Systems Architect, Lead Developer, User

## Context and Problem Statement

Downstream games and engine subsystems (ECS, Physics, Renderer, Camera) require core mathematical structures: 2D and 3D vectors, 4x4 transformation matrices, and spatial transform hierarchies.

In the reference repository (`B50calabrese/GameEngine`), math was implemented by dumping the entire third-party `glm` header tree into `third_party/include/glm` and including `<glm/glm.hpp>` directly across public engine headers (`primitive_renderer.h`, `camera.h`, `renderer.h`). This created significant architectural issues:
1. **Severe Leaky Abstraction**: Downstream submodule consumers were forced to configure GLM include paths and had compiler compilation times bloated by thousands of lines of template instantiations.
2. **Style Inconsistency**: GLM uses standard C++ lowercase / snake_case conventions (`glm::vec2`, `glm::rotate`, `glm::ortho`), violating Google C++ Style Guide conventions (`PascalCase` types and methods).
3. **No Ergonomic Transform Hierarchy**: No unified `Transform` component linking 2D and 3D representations cleanly.

## Decision Drivers

* **Submodule-First**: Public headers under `include/engine/` must remain completely self-contained.
* **Strict Google C++ Style**: Clean naming (`Vector2`, `Matrix4`, `Transform`, `Dot()`, `Cross()`, `Normalized()`, `ToMatrix()`).
* **Column-Major Alignment**: Matches OpenGL shaders natively with zero memory reordering.
* **Dual 2D/3D Support**: 2D ergonomics with full 3D mathematical foundations.

## Considered Options

1. **Option 1: Vendor & Expose GLM Publicly**: Follow reference repo. (Rejected: leaky vendor headers, high compile-time overhead, non-conforming style).
2. **Option 2: Internal GLM with Opaque Facade Wrappers**: Wrap GLM internally behind abstract interfaces. (Rejected: unnecessary heap or pointer indirection for simple POD vectors).
3. **Option 3: Native Lightweight C++17 Math Library (Selected)**: Implement dedicated `Vector2`, `Vector3`, `Vector4`, `Matrix4`, and `Transform` types with full operator overloads, projection factories, and inversion algorithms.

## Decision Outcome

Option 3 was selected and implemented:
* Native `engine::math` namespace.
* Fast, inlined, pass-by-value vector types.
* Full 4x4 matrix inverse (cofactor method), determinant, orthographic, perspective, and LookAt view transformations.
* Integrated seamlessly with `PrimitiveRenderer` and future ECS/Physics subsystems.
