#include "engine/renderer/primitive_renderer.h"

#include <cmath>
#include <cstring>
#include <vector>

#include "engine/core/assert.h"
#include "engine/core/log.h"
#include "engine/renderer/render_command.h"
#include "engine/renderer/shader.h"

namespace engine::renderer {

namespace {

struct PrimitiveVertex {
  float position[2];
  float color[4];
};

constexpr std::uint32_t kMaxQuads = 5000;
constexpr std::uint32_t kMaxVertices = kMaxQuads * 4;
constexpr std::uint32_t kMaxIndices = kMaxQuads * 6;

static const char* kPrimitiveVertexShader = R"(
#version 330 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main() {
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
}
)";

static const char* kPrimitiveFragmentShader = R"(
#version 330 core
in vec4 v_Color;
out vec4 o_Color;

void main() {
    o_Color = v_Color;
}
)";

struct RendererData {
  std::shared_ptr<VertexArray> vertex_array;
  std::shared_ptr<VertexBuffer> vertex_buffer;
  std::shared_ptr<Shader> shader;

  std::vector<PrimitiveVertex> vertex_buffer_cpu;
  std::uint32_t index_count = 0;

  float current_view_projection[16] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  PrimitiveRenderer::Statistics stats;
  bool is_initialized = false;
};

static RendererData s_data;

}  // namespace

bool PrimitiveRenderer::Initialize() {
  if (s_data.is_initialized) {
    return true;
  }

  if (!RenderCommand::Initialize()) {
    ENGINE_LOG_WARN("PrimitiveRenderer: Hardware context not available, running in fallback mode.");
    return false;
  }

  s_data.vertex_array = VertexArray::Create();

  s_data.vertex_buffer = VertexBuffer::Create(kMaxVertices * sizeof(PrimitiveVertex));
  s_data.vertex_buffer->SetLayout({
      {ShaderDataType::Float2, "a_Position"},
      {ShaderDataType::Float4, "a_Color"}
  });
  s_data.vertex_array->AddVertexBuffer(s_data.vertex_buffer);

  s_data.vertex_buffer_cpu.reserve(kMaxVertices);

  // Pre-generate static quad indices: 0, 1, 2, 2, 3, 0 (offset by 4 per quad)
  std::vector<std::uint32_t> indices(kMaxIndices);
  std::uint32_t offset = 0;
  for (std::size_t i = 0; i < kMaxIndices; i += 6) {
    indices[i + 0] = offset + 0;
    indices[i + 1] = offset + 1;
    indices[i + 2] = offset + 2;
    indices[i + 3] = offset + 2;
    indices[i + 4] = offset + 3;
    indices[i + 5] = offset + 0;
    offset += 4;
  }

  auto index_buffer = IndexBuffer::Create(indices.data(), kMaxIndices);
  s_data.vertex_array->SetIndexBuffer(index_buffer);

  s_data.shader = Shader::Create("PrimitiveShader", kPrimitiveVertexShader,
                                 kPrimitiveFragmentShader);

  s_data.is_initialized = true;
  ENGINE_LOG_INFO("PrimitiveRenderer initialized successfully.");
  return true;
}

void PrimitiveRenderer::Shutdown() {
  s_data.vertex_array.reset();
  s_data.vertex_buffer.reset();
  s_data.shader.reset();
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
  s_data.is_initialized = false;
  ENGINE_LOG_INFO("PrimitiveRenderer shut down.");
}

void PrimitiveRenderer::BeginBatch(float view_width, float view_height, bool y_down) {
  // Construct orthographic projection matrix [4x4 column-major]
  float proj[16] = {0};

  proj[0] = 2.0f / view_width;
  proj[10] = -1.0f;
  proj[12] = -1.0f;
  proj[15] = 1.0f;

  if (y_down) {
    // (0,0) Top-Left, Y pointing down
    proj[5] = -2.0f / view_height;
    proj[13] = 1.0f;
  } else {
    // (0,0) Bottom-Left, Y pointing up
    proj[5] = 2.0f / view_height;
    proj[13] = -1.0f;
  }

  BeginBatch(proj);
}

void PrimitiveRenderer::BeginBatch(const float* projection_matrix) {
  std::memcpy(s_data.current_view_projection, projection_matrix, 16 * sizeof(float));
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
}

void PrimitiveRenderer::EndBatch() {
  Flush();
}

void PrimitiveRenderer::Flush() {
  if (s_data.index_count == 0 || s_data.vertex_buffer_cpu.empty()) {
    return;
  }

  if (!s_data.is_initialized) {
    return;
  }

  s_data.vertex_buffer->SetData(
      s_data.vertex_buffer_cpu.data(),
      static_cast<std::uint32_t>(s_data.vertex_buffer_cpu.size() * sizeof(PrimitiveVertex)));

  s_data.shader->Bind();
  s_data.shader->SetMat4("u_ViewProjection", s_data.current_view_projection);

  RenderCommand::DrawIndexed(s_data.vertex_array, s_data.index_count);
  ++s_data.stats.draw_calls;
}

void PrimitiveRenderer::NextBatch() {
  Flush();
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
}

void PrimitiveRenderer::DrawQuad(float x, float y, float width, float height,
                                 const Color& color) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  PrimitiveVertex v0{{x, y}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v1{{x + width, y}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v2{{x + width, y + height}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v3{{x, y + height}, {color.r, color.g, color.b, color.a}};

  s_data.vertex_buffer_cpu.push_back(v0);
  s_data.vertex_buffer_cpu.push_back(v1);
  s_data.vertex_buffer_cpu.push_back(v2);
  s_data.vertex_buffer_cpu.push_back(v3);

  s_data.index_count += 6;
  ++s_data.stats.quad_count;
}

void PrimitiveRenderer::DrawRotatedQuad(float x, float y, float width, float height,
                                        float rotation_radians, const Color& color) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  float cx = x + width * 0.5f;
  float cy = y + height * 0.5f;
  float hw = width * 0.5f;
  float hh = height * 0.5f;

  float cos_r = std::cos(rotation_radians);
  float sin_r = std::sin(rotation_radians);

  float offsets[4][2] = {
      {-hw, -hh},
      {hw, -hh},
      {hw, hh},
      {-hw, hh}
  };

  for (int i = 0; i < 4; ++i) {
    float rx = cx + offsets[i][0] * cos_r - offsets[i][1] * sin_r;
    float ry = cy + offsets[i][0] * sin_r + offsets[i][1] * cos_r;
    s_data.vertex_buffer_cpu.push_back({{rx, ry}, {color.r, color.g, color.b, color.a}});
  }

  s_data.index_count += 6;
  ++s_data.stats.quad_count;
}

void PrimitiveRenderer::DrawTriangle(float x1, float y1, float x2, float y2,
                                     float x3, float y3, const Color& color) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  PrimitiveVertex v0{{x1, y1}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v1{{x2, y2}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v2{{x3, y3}, {color.r, color.g, color.b, color.a}};
  // Degenerate fourth vertex: discarded without drawing by triangle index 2,2,0
  PrimitiveVertex v3 = v2;

  s_data.vertex_buffer_cpu.push_back(v0);
  s_data.vertex_buffer_cpu.push_back(v1);
  s_data.vertex_buffer_cpu.push_back(v2);
  s_data.vertex_buffer_cpu.push_back(v3);

  s_data.index_count += 6;
  ++s_data.stats.triangle_count;
}

void PrimitiveRenderer::DrawLine(float x1, float y1, float x2, float y2,
                                 const Color& color, float line_width) {
  float dx = x2 - x1;
  float dy = y2 - y1;
  float length = std::sqrt(dx * dx + dy * dy);
  if (length < 0.0001f) {
    return;
  }

  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  float nx = -dy / length;
  float ny = dx / length;
  float hw = line_width * 0.5f;

  PrimitiveVertex v0{{x1 - nx * hw, y1 - ny * hw}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v1{{x2 - nx * hw, y2 - ny * hw}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v2{{x2 + nx * hw, y2 + ny * hw}, {color.r, color.g, color.b, color.a}};
  PrimitiveVertex v3{{x1 + nx * hw, y1 + ny * hw}, {color.r, color.g, color.b, color.a}};

  s_data.vertex_buffer_cpu.push_back(v0);
  s_data.vertex_buffer_cpu.push_back(v1);
  s_data.vertex_buffer_cpu.push_back(v2);
  s_data.vertex_buffer_cpu.push_back(v3);

  s_data.index_count += 6;
  ++s_data.stats.line_count;
}

void PrimitiveRenderer::DrawRectOutline(float x, float y, float width, float height,
                                        const Color& color, float line_width) {
  DrawLine(x, y, x + width, y, color, line_width);
  DrawLine(x + width, y, x + width, y + height, color, line_width);
  DrawLine(x + width, y + height, x, y + height, color, line_width);
  DrawLine(x, y + height, x, y, color, line_width);
}

void PrimitiveRenderer::DrawCircle(float center_x, float center_y, float radius,
                                   const Color& color, std::uint32_t segments) {
  segments = std::clamp(segments, 8u, 128u);
  constexpr float kTwoPi = 6.28318530718f;
  float step = kTwoPi / static_cast<float>(segments);

  for (std::uint32_t i = 0; i < segments; ++i) {
    float angle1 = static_cast<float>(i) * step;
    float angle2 = static_cast<float>(i + 1) * step;

    float x1 = center_x + radius * std::cos(angle1);
    float y1 = center_y + radius * std::sin(angle1);
    float x2 = center_x + radius * std::cos(angle2);
    float y2 = center_y + radius * std::sin(angle2);

    DrawTriangle(center_x, center_y, x1, y1, x2, y2, color);
  }
}

const PrimitiveRenderer::Statistics& PrimitiveRenderer::GetStats() {
  return s_data.stats;
}

void PrimitiveRenderer::ResetStats() {
  s_data.stats = Statistics();
}

}  // namespace engine::renderer
