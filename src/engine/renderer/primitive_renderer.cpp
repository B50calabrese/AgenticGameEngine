#include "engine/renderer/primitive_renderer.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <vector>

#include "engine/core/assert.h"
#include "engine/core/log.h"
#include "engine/math/matrix4.h"
#include "engine/math/vector2.h"
#include "engine/renderer/render_command.h"
#include "engine/renderer/shader.h"

namespace engine::renderer {

namespace {

struct PrimitiveVertex {
  float position[2];
  float color[4];
  float tex_coord[2];
  float tex_index;
};

constexpr std::uint32_t kMaxQuads = 5000;
constexpr std::uint32_t kMaxVertices = kMaxQuads * 4;
constexpr std::uint32_t kMaxIndices = kMaxQuads * 6;
constexpr std::uint32_t kMaxTextureSlots = 16;

static const char* kPrimitiveVertexShader = R"(
#version 330 core
layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_ViewProjection * vec4(a_Position, 0.0, 1.0);
}
)";

static const char* kPrimitiveFragmentShader = R"(
#version 330 core
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[16];

out vec4 o_Color;

void main() {
    vec4 tex_color = vec4(1.0);
    int idx = int(v_TexIndex + 0.5);
    switch (idx) {
        case 0:  tex_color = texture(u_Textures[0],  v_TexCoord); break;
        case 1:  tex_color = texture(u_Textures[1],  v_TexCoord); break;
        case 2:  tex_color = texture(u_Textures[2],  v_TexCoord); break;
        case 3:  tex_color = texture(u_Textures[3],  v_TexCoord); break;
        case 4:  tex_color = texture(u_Textures[4],  v_TexCoord); break;
        case 5:  tex_color = texture(u_Textures[5],  v_TexCoord); break;
        case 6:  tex_color = texture(u_Textures[6],  v_TexCoord); break;
        case 7:  tex_color = texture(u_Textures[7],  v_TexCoord); break;
        case 8:  tex_color = texture(u_Textures[8],  v_TexCoord); break;
        case 9:  tex_color = texture(u_Textures[9],  v_TexCoord); break;
        case 10: tex_color = texture(u_Textures[10], v_TexCoord); break;
        case 11: tex_color = texture(u_Textures[11], v_TexCoord); break;
        case 12: tex_color = texture(u_Textures[12], v_TexCoord); break;
        case 13: tex_color = texture(u_Textures[13], v_TexCoord); break;
        case 14: tex_color = texture(u_Textures[14], v_TexCoord); break;
        case 15: tex_color = texture(u_Textures[15], v_TexCoord); break;
    }
    o_Color = tex_color * v_Color;
}
)";

struct RendererData {
  std::shared_ptr<VertexArray> vertex_array;
  std::shared_ptr<VertexBuffer> vertex_buffer;
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture2D> white_texture;

  std::vector<PrimitiveVertex> vertex_buffer_cpu;
  std::uint32_t index_count = 0;

  std::array<std::shared_ptr<Texture2D>, kMaxTextureSlots> texture_slots;
  std::uint32_t texture_slot_index = 1;  // Slot 0 reserved for white_texture

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

float GetOrAssignTextureSlot(const std::shared_ptr<Texture2D>& texture) {
  if (!texture || !texture->IsLoaded()) {
    return 0.0f;
  }

  for (std::uint32_t i = 1; i < s_data.texture_slot_index; ++i) {
    if (*s_data.texture_slots[i] == *texture) {
      return static_cast<float>(i);
    }
  }

  if (s_data.texture_slot_index >= kMaxTextureSlots) {
    PrimitiveRenderer::Flush();
    s_data.vertex_buffer_cpu.clear();
    s_data.index_count = 0;
    s_data.texture_slot_index = 1;
  }

  s_data.texture_slots[s_data.texture_slot_index] = texture;
  float assigned = static_cast<float>(s_data.texture_slot_index);
  ++s_data.texture_slot_index;
  return assigned;
}

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
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
      {ShaderDataType::Float,  "a_TexIndex"}
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

  // 1x1 white texture at slot 0 for solid-colored geometry
  TextureSpecification white_spec;
  white_spec.width = 1;
  white_spec.height = 1;
  white_spec.format = TextureFormat::RGBA8;
  white_spec.generate_mips = false;
  s_data.white_texture = Texture2D::Create(white_spec);
  std::uint32_t white_pixel = 0xFFFFFFFF;
  s_data.white_texture->SetData(&white_pixel, sizeof(white_pixel));
  s_data.texture_slots[0] = s_data.white_texture;

  s_data.shader = Shader::Create("PrimitiveShader", kPrimitiveVertexShader,
                                 kPrimitiveFragmentShader);
  if (s_data.shader) {
    s_data.shader->Bind();
    for (std::uint32_t i = 0; i < kMaxTextureSlots; ++i) {
      s_data.shader->SetInt("u_Textures[" + std::to_string(i) + "]",
                            static_cast<int>(i));
    }
  }

  s_data.is_initialized = true;
  ENGINE_LOG_INFO("PrimitiveRenderer initialized successfully.");
  return true;
}

void PrimitiveRenderer::Shutdown() {
  for (auto& slot : s_data.texture_slots) {
    slot.reset();
  }
  s_data.white_texture.reset();
  s_data.vertex_array.reset();
  s_data.vertex_buffer.reset();
  s_data.shader.reset();
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
  s_data.texture_slot_index = 1;
  s_data.is_initialized = false;
  ENGINE_LOG_INFO("PrimitiveRenderer shut down.");
}

void PrimitiveRenderer::BeginBatch(float view_width, float view_height, bool y_down) {
  float proj[16] = {0};

  proj[0] = 2.0f / view_width;
  proj[10] = -1.0f;
  proj[12] = -1.0f;
  proj[15] = 1.0f;

  if (y_down) {
    proj[5] = -2.0f / view_height;
    proj[13] = 1.0f;
  } else {
    proj[5] = 2.0f / view_height;
    proj[13] = -1.0f;
  }

  BeginBatch(proj);
}

void PrimitiveRenderer::BeginBatch(const float* projection_matrix) {
  std::memcpy(s_data.current_view_projection, projection_matrix, 16 * sizeof(float));
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
  s_data.texture_slot_index = 1;
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

  for (std::uint32_t i = 0; i < s_data.texture_slot_index; ++i) {
    if (s_data.texture_slots[i]) {
      s_data.texture_slots[i]->Bind(i);
    }
  }

  s_data.shader->Bind();
  s_data.shader->SetMat4("u_ViewProjection", s_data.current_view_projection);

  RenderCommand::DrawIndexed(s_data.vertex_array, s_data.index_count);
  ++s_data.stats.draw_calls;
}

void PrimitiveRenderer::NextBatch() {
  Flush();
  s_data.vertex_buffer_cpu.clear();
  s_data.index_count = 0;
  s_data.texture_slot_index = 1;
}

void PrimitiveRenderer::DrawQuad(float x, float y, float width, float height,
                                 const Color& color) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  s_data.vertex_buffer_cpu.push_back({{x, y}, {color.r, color.g, color.b, color.a}, {0.0f, 0.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x + width, y}, {color.r, color.g, color.b, color.a}, {1.0f, 0.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x + width, y + height}, {color.r, color.g, color.b, color.a}, {1.0f, 1.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x, y + height}, {color.r, color.g, color.b, color.a}, {0.0f, 1.0f}, 0.0f});

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

  float offsets[4][2] = {{-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh}};
  float uvs[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

  for (int i = 0; i < 4; ++i) {
    float rx = cx + offsets[i][0] * cos_r - offsets[i][1] * sin_r;
    float ry = cy + offsets[i][0] * sin_r + offsets[i][1] * cos_r;
    s_data.vertex_buffer_cpu.push_back(
        {{rx, ry}, {color.r, color.g, color.b, color.a}, {uvs[i][0], uvs[i][1]}, 0.0f});
  }

  s_data.index_count += 6;
  ++s_data.stats.quad_count;
}

void PrimitiveRenderer::DrawTriangle(float x1, float y1, float x2, float y2,
                                     float x3, float y3, const Color& color) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  PrimitiveVertex v0{{x1, y1}, {color.r, color.g, color.b, color.a}, {0.0f, 0.0f}, 0.0f};
  PrimitiveVertex v1{{x2, y2}, {color.r, color.g, color.b, color.a}, {1.0f, 0.0f}, 0.0f};
  PrimitiveVertex v2{{x3, y3}, {color.r, color.g, color.b, color.a}, {0.5f, 1.0f}, 0.0f};
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

  s_data.vertex_buffer_cpu.push_back({{x1 - nx * hw, y1 - ny * hw}, {color.r, color.g, color.b, color.a}, {0.0f, 0.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x2 - nx * hw, y2 - ny * hw}, {color.r, color.g, color.b, color.a}, {1.0f, 0.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x2 + nx * hw, y2 + ny * hw}, {color.r, color.g, color.b, color.a}, {1.0f, 1.0f}, 0.0f});
  s_data.vertex_buffer_cpu.push_back({{x1 + nx * hw, y1 + ny * hw}, {color.r, color.g, color.b, color.a}, {0.0f, 1.0f}, 0.0f});

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

void PrimitiveRenderer::BeginBatch(const engine::math::Matrix4& view_projection) {
  BeginBatch(view_projection.Data());
}

void PrimitiveRenderer::DrawQuad(const engine::math::Vector2& position,
                                 const engine::math::Vector2& size,
                                 const Color& color) {
  DrawQuad(position.x, position.y, size.x, size.y, color);
}

void PrimitiveRenderer::DrawRotatedQuad(const engine::math::Vector2& position,
                                        const engine::math::Vector2& size,
                                        float rotation_radians,
                                        const Color& color) {
  DrawRotatedQuad(position.x, position.y, size.x, size.y, rotation_radians, color);
}

void PrimitiveRenderer::DrawTriangle(const engine::math::Vector2& p1,
                                     const engine::math::Vector2& p2,
                                     const engine::math::Vector2& p3,
                                     const Color& color) {
  DrawTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
}

void PrimitiveRenderer::DrawLine(const engine::math::Vector2& start,
                                 const engine::math::Vector2& end,
                                 const Color& color, float line_width) {
  DrawLine(start.x, start.y, end.x, end.y, color, line_width);
}

void PrimitiveRenderer::DrawCircle(const engine::math::Vector2& center,
                                   float radius, const Color& color,
                                   std::uint32_t segments) {
  DrawCircle(center.x, center.y, radius, color, segments);
}

void PrimitiveRenderer::DrawTexturedQuad(const engine::math::Vector2& position,
                                         const engine::math::Vector2& size,
                                         const std::shared_ptr<Texture2D>& texture,
                                         const Color& tint) {
  DrawTexturedQuad(position, size, texture,
                   engine::math::Vector2(0.0f, 0.0f),
                   engine::math::Vector2(1.0f, 1.0f), 0.0f, tint);
}

void PrimitiveRenderer::DrawTexturedQuad(const engine::math::Vector2& position,
                                         const engine::math::Vector2& size,
                                         const std::shared_ptr<Texture2D>& texture,
                                         float rotation_radians,
                                         const Color& tint) {
  DrawTexturedQuad(position, size, texture,
                   engine::math::Vector2(0.0f, 0.0f),
                   engine::math::Vector2(1.0f, 1.0f), rotation_radians, tint);
}

void PrimitiveRenderer::DrawTexturedQuad(const engine::math::Vector2& position,
                                         const engine::math::Vector2& size,
                                         const std::shared_ptr<Texture2D>& texture,
                                         const engine::math::Vector2& uv_min,
                                         const engine::math::Vector2& uv_max,
                                         float rotation_radians,
                                         const Color& tint) {
  if (s_data.index_count + 6 > kMaxIndices) {
    NextBatch();
  }

  float tex_index = GetOrAssignTextureSlot(texture);

  float cx = position.x + size.x * 0.5f;
  float cy = position.y + size.y * 0.5f;
  float hw = size.x * 0.5f;
  float hh = size.y * 0.5f;

  float cos_r = std::cos(rotation_radians);
  float sin_r = std::sin(rotation_radians);

  float offsets[4][2] = {{-hw, -hh}, {hw, -hh}, {hw, hh}, {-hw, hh}};
  float uvs[4][2] = {
      {uv_min.x, uv_min.y},
      {uv_max.x, uv_min.y},
      {uv_max.x, uv_max.y},
      {uv_min.x, uv_max.y}
  };

  for (int i = 0; i < 4; ++i) {
    float rx = (rotation_radians != 0.0f)
                   ? (cx + offsets[i][0] * cos_r - offsets[i][1] * sin_r)
                   : (cx + offsets[i][0]);
    float ry = (rotation_radians != 0.0f)
                   ? (cy + offsets[i][0] * sin_r + offsets[i][1] * cos_r)
                   : (cy + offsets[i][1]);

    s_data.vertex_buffer_cpu.push_back(
        {{rx, ry}, {tint.r, tint.g, tint.b, tint.a}, {uvs[i][0], uvs[i][1]}, tex_index});
  }

  s_data.index_count += 6;
  ++s_data.stats.quad_count;
}

const PrimitiveRenderer::Statistics& PrimitiveRenderer::GetStats() {
  return s_data.stats;
}

void PrimitiveRenderer::ResetStats() {
  s_data.stats = Statistics();
}

}  // namespace engine::renderer
