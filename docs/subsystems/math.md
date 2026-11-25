# Math Subsystem

The **Math** subsystem provides core linear algebra, transformation, and geometric calculations for the Agentic Game Engine. Designed to support fast 2D gameplay while naturally scaling to 2.5D and 3D rendering, it adheres strictly to Google C++ Style and zero-vendor-leakage principles.

---

## 1. Design Principles

1. **Submodule Isolation (No Vendor Leakage)**:
   Downstream games consuming `engine::engine` never need external math libraries (e.g. GLM) in their include paths. The engine's public math headers live exclusively in `include/engine/math/`.
2. **Column-Major Matrix Layout**:
   `Matrix4` stores elements in standard column-major order `elements[col * 4 + row]`, matching OpenGL shaders (`glUniformMatrix4fv` with `GL_FALSE`).
3. **2D & 3D Unified Transformations**:
   `Transform` encapsulates translation, rotation, and scaling in 3D while providing ergonomic 2D convenience methods (`SetPosition2D`, `SetRotation2D`, `SetScale2D`).
4. **Constexpr & Value Semantics**:
   Vector types are lightweight Plain Old Data (POD) structures designed for pass-by-value efficiency, SIMD vectorization friendliness, and compile-time evaluation where possible.

---

## 2. Public API Surface

All math primitives live in namespace `engine::math`:

| Header | Type | Description |
| :--- | :--- | :--- |
| `math_utils.h` | Utilities & Constants | Constants (`kPi`, `kHalfPi`, `kTwoPi`, `kDegToRad`, `kRadToDeg`, `kEpsilon`), `ToRadians`, `ToDegrees`, `Clamp`, `Lerp`, and `Approximately`. |
| `vector2.h` | `Vector2` | 2D vector for positions, sizes, velocities, and directions. Operators `+`, `-`, `*`, `/`, `Dot`, `Cross` (scalar), `Length`, `Normalized`, `Perpendicular`, `Angle`, `Distance`, `Lerp`. |
| `vector3.h` | `Vector3` | 3D vector for 3D/2.5D coordinates, directions, scales, and normals. `Dot`, `Cross` (vector), `Length`, `Normalized`, `Distance`, `Lerp`, `ToVector2`. |
| `vector4.h` | `Vector4` | 4D vector for homogeneous coordinates and clip-space math. |
| `matrix4.h` | `Matrix4` | 4x4 column-major transformation matrix. `TransformPoint`, `TransformDirection`, `Transposed`, `Inverted`, `Determinant`, `Orthographic`, `Perspective`, `LookAt`, `Translation`, `Rotation`, `Scale`. |
| `transform.h` | `Transform` | Spatial representation ($T \times R \times S$). Produces local matrix via `ToMatrix()` and inverse via `ToInverseMatrix()`. |

---

## 3. Usage Examples

### 2D Coordinate & Vector Arithmetic
```cpp
#include "engine/math/vector2.h"

using engine::math::Vector2;

Vector2 position(100.0f, 200.0f);
Vector2 velocity(10.0f, 0.0f);
float delta_time = 0.016f;

position += velocity * delta_time;

// Direction & Distance
Vector2 target(480.0f, 270.0f);
Vector2 to_target = (target - position).Normalized();
float distance = position.Distance(target);
```

### Orthographic 2D Camera Projection
```cpp
#include "engine/math/matrix4.h"

// Screen coordinate orthographic matrix: Left=0, Right=960, Bottom=0, Top=540
auto projection = engine::math::Matrix4::Orthographic(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

// Transform a screen point to Normalized Device Coordinates (NDC) [-1, 1]
engine::math::Vector3 ndc = projection.TransformPoint(engine::math::Vector3(480.0f, 270.0f, 0.0f));
// ndc == (0, 0, 0)
```

### Spatial Transform Hierarchy
```cpp
#include "engine/math/transform.h"

engine::math::Transform player_transform;
player_transform.SetPosition2D(150.0f, 300.0f);
player_transform.SetRotation2D(engine::math::ToRadians(45.0f));
player_transform.SetScale2D(2.0f, 2.0f);

// Get transformation matrix for rendering or physics
engine::math::Matrix4 model_matrix = player_transform.ToMatrix();
```

---

## 4. Integration with Renderer

The `PrimitiveRenderer` natively accepts `Matrix4` and `Vector2`:
```cpp
engine::renderer::PrimitiveRenderer::BeginBatch(projection);
engine::renderer::PrimitiveRenderer::DrawQuad(
    player_transform.GetPosition2D(),
    engine::math::Vector2(64.0f, 64.0f),
    engine::renderer::Color::Green()
);
engine::renderer::PrimitiveRenderer::EndBatch();
```
