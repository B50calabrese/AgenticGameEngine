# Renderer Subsystem

The **Renderer** subsystem provides hardware-accelerated graphics capabilities for the Agentic Game Engine. It is architected in two clean layers:
1. **Hardware Abstraction Layer (HAL)**: Platform-agnostic C++ abstractions over GPU buffers, vertex arrays, shaders, and draw execution commands.
2. **2D Primitive Renderer**: High-performance, zero-allocation batched drawing of 2D geometric shapes (quads, rotated quads, triangles, lines, rect outlines, and circles).

---

## 1. Architecture & Layering

```text
┌───────────────────────────────────────────────────────────────┐
│                    Downstream Game / Demo                     │
└───────────────────────────────┬───────────────────────────────┘
                                │
┌───────────────────────────────▼───────────────────────────────┐
│              High-Level: PrimitiveRenderer                    │
│   (DrawQuad, DrawRotatedQuad, DrawTriangle, DrawLine, etc.)   │
└───────────────────────────────┬───────────────────────────────┘
                                │
┌───────────────────────────────▼───────────────────────────────┐
│            Hardware Abstraction Layer (HAL)                   │
│   ┌──────────────┐  ┌──────────────┐  ┌───────────────────┐   │
│   │ VertexArray  │  │ VertexBuffer │  │    IndexBuffer    │   │
│   └──────────────┘  └──────────────┘  └───────────────────┘   │
│   ┌──────────────┐  ┌──────────────┐                          │
│   │    Shader    │  │ RenderCommand│                          │
│   └──────────────┘  └──────────────┘                          │
└───────────────────────────────┬───────────────────────────────┘
                                │
┌───────────────────────────────▼───────────────────────────────┐
│           Platform Driver / OpenGL 3.3 Core Profile           │
│                   (GLAD Function Loader)                      │
└───────────────────────────────────────────────────────────────┘
```

---

## 2. Public API Surface

Public headers live exclusively in `include/engine/renderer/`:

| Header | Class / Struct | Description |
| :--- | :--- | :--- |
| `color.h` | `Color` | Normalized RGBA float representation [0.0, 1.0] with hex parsing, clamping arithmetic, and color presets (`Red()`, `Cyan()`, etc.). |
| `buffer.h` | `VertexBuffer`, `IndexBuffer`, `BufferLayout` | RAII GPU buffer interfaces with dynamic memory upload and automatic stride/offset calculation. |
| `vertex_array.h` | `VertexArray` | VAO state binding vertex buffers with attribute pointers and index buffer. |
| `shader.h` | `Shader` | GLSL program compilation, linking, and uniform dispatch (`SetInt`, `SetFloat`, `SetMat4`). |
| `texture.h` | `Texture2D`, `TextureSpecification` | 2D GPU texture abstraction supporting procedural memory upload (`SetData`) and image file loading (`stb_image`). |
| `render_command.h` | `RenderCommand` | Low-level context initialization (GLAD), clear color, viewport, and indexed/line draw execution. |
| `primitive_renderer.h`| `PrimitiveRenderer` | Batched 2D geometry & textured quad renderer (up to 16 texture slots per batch) with frame statistics. |
| `render_queue.h` | `RenderQueue` | Z-sorted submission queue that orders draw calls back-to-front before flushing through `PrimitiveRenderer`. |
| `font.h` | `Font`, `GlyphMetrics` | TrueType font loading (`stb_truetype`) and bitmap/rotated string rendering (`DrawString`). |

---

## 3. Data-Oriented Batching Strategy

`PrimitiveRenderer` implements data-oriented batching:
* **Preallocated Memory**: A pre-allocated CPU buffer (`std::vector<PrimitiveVertex>`) with pre-calculated quad indices $(0, 1, 2, 2, 3, 0)$ eliminates heap allocation churn during the game loop.
* **Unified Geometry**: Triangles are submitted with degenerate fourth vertices and discarded by the GPU with zero overhead, allowing quads and triangles to batch together into a single draw call.
* **Arbitrary Thickness Lines**: Drawn as oriented quads with computed normals, ensuring cross-platform line thickness support without relying on deprecated `glLineWidth`.
* **Zero Vendor Leakage**: Downstream consumers never include GLAD, OpenGL, or GLFW headers.

---

## 4. Usage Example

```cpp
#include "engine/engine.h"

// 1. Initialize context & primitive renderer
engine::renderer::PrimitiveRenderer::Initialize();
engine::renderer::RenderCommand::SetViewport(0, 0, window->GetWidth(), window->GetHeight());

// 2. Clear frame buffer
engine::renderer::RenderCommand::SetClearColor(engine::renderer::Color(0.1f, 0.12f, 0.16f, 1.0f));
engine::renderer::RenderCommand::Clear();

// 3. Begin batch in pixel space (e.g. 960x540)
engine::renderer::PrimitiveRenderer::BeginBatch(960.0f, 540.0f);

// 4. Draw geometric primitives
engine::renderer::PrimitiveRenderer::DrawQuad(100.0f, 100.0f, 200.0f, 150.0f, engine::renderer::Color::Red());
engine::renderer::PrimitiveRenderer::DrawCircle(480.0f, 270.0f, 60.0f, engine::renderer::Color::Cyan());
engine::renderer::PrimitiveRenderer::DrawLine(0.0f, 0.0f, 960.0f, 540.0f, engine::renderer::Color::Yellow(), 3.0f);

// 5. Flush batch to GPU
engine::renderer::PrimitiveRenderer::EndBatch();
```

### Textures, Z-Sorted RenderQueue & Font Rendering

```cpp
// Load a texture from disk or create a procedural texture
auto sprite = engine::renderer::Texture2D::Create("assets/textures/card.png");

// Load a TrueType font
auto font = engine::renderer::Font::Load("assets/fonts/Roboto-Regular.ttf", 24);

// Submit Z-ordered quads to RenderQueue (back-to-front)
engine::renderer::RenderQueue queue;
queue.SubmitQuad({100.0f, 100.0f}, {200.0f, 150.0f},
                 engine::renderer::Color::DarkGray(), /*z_order=*/0.0f);
queue.SubmitTexturedQuad({120.0f, 120.0f}, {128.0f, 128.0f}, sprite,
                         engine::renderer::Color::White(), /*z_order=*/1.0f);
queue.Flush(engine::math::Matrix4::Orthographic(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f));

// Draw horizontal or rotated text
engine::renderer::PrimitiveRenderer::BeginBatch(960.0f, 540.0f);
font->DrawString("Score: 100", {20.0f, 500.0f}, engine::renderer::Color::White());
font->DrawString("Rotated!", {480.0f, 270.0f}, engine::renderer::Color::Yellow(), 0.25f);
engine::renderer::PrimitiveRenderer::EndBatch();
```

---

## 5. Demo Validation

Run the renderer demos:
```bash
make primitives-demo
make textures-demo
make font-demo
```
For automated headless testing:
```bash
./build/demos/primitives_demo --frames 60
./build/demos/textures_demo --frames 10
./build/demos/font_demo --frames 10
```
