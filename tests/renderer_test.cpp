#include <gtest/gtest.h>

#include "engine/renderer/buffer.h"
#include "engine/renderer/color.h"
#include "engine/renderer/primitive_renderer.h"

namespace {

using engine::renderer::BufferElement;
using engine::renderer::BufferLayout;
using engine::renderer::Color;
using engine::renderer::PrimitiveRenderer;
using engine::renderer::ShaderDataType;
using engine::renderer::ShaderDataTypeSize;

TEST(ColorTest, DefaultColorIsOpaqueWhite) {
  Color c;
  EXPECT_FLOAT_EQ(c.r, 1.0f);
  EXPECT_FLOAT_EQ(c.g, 1.0f);
  EXPECT_FLOAT_EQ(c.b, 1.0f);
  EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(ColorTest, FromRgba8ConvertsCorrectly) {
  Color c = Color::FromRgba8(255, 128, 0, 255);
  EXPECT_FLOAT_EQ(c.r, 1.0f);
  EXPECT_NEAR(c.g, 128.0f / 255.0f, 0.001f);
  EXPECT_FLOAT_EQ(c.b, 0.0f);
  EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(ColorTest, FromHexConvertsCorrectly) {
  // 0xFF8000FF -> R=255, G=128, B=0, A=255
  Color c = Color::FromHex(0xFF8000FF);
  EXPECT_FLOAT_EQ(c.r, 1.0f);
  EXPECT_NEAR(c.g, 128.0f / 255.0f, 0.001f);
  EXPECT_FLOAT_EQ(c.b, 0.0f);
  EXPECT_FLOAT_EQ(c.a, 1.0f);
}

TEST(ColorTest, EqualityOperators) {
  Color c1(0.5f, 0.2f, 0.8f, 1.0f);
  Color c2(0.5f, 0.2f, 0.8f, 1.0f);
  Color c3(0.5f, 0.2f, 0.7f, 1.0f);

  EXPECT_EQ(c1, c2);
  EXPECT_NE(c1, c3);
}

TEST(ColorTest, ArithmeticClampsToUnitRange) {
  Color c(0.8f, 0.5f, 0.2f, 1.0f);
  Color added = c + Color(0.5f, 0.7f, 0.9f, 0.0f);
  EXPECT_FLOAT_EQ(added.r, 1.0f);
  EXPECT_FLOAT_EQ(added.g, 1.0f);
  EXPECT_FLOAT_EQ(added.b, 1.0f);

  Color scaled = c * 2.0f;
  EXPECT_FLOAT_EQ(scaled.r, 1.0f);
  EXPECT_FLOAT_EQ(scaled.g, 1.0f);
  EXPECT_FLOAT_EQ(scaled.b, 0.4f);
}

TEST(ColorTest, StandardPresetsAreCorrect) {
  EXPECT_EQ(Color::Red(), Color(1.0f, 0.0f, 0.0f, 1.0f));
  EXPECT_EQ(Color::Green(), Color(0.0f, 1.0f, 0.0f, 1.0f));
  EXPECT_EQ(Color::Blue(), Color(0.0f, 0.0f, 1.0f, 1.0f));
  EXPECT_EQ(Color::Black(), Color(0.0f, 0.0f, 0.0f, 1.0f));
  EXPECT_EQ(Color::Clear(), Color(0.0f, 0.0f, 0.0f, 0.0f));
}

TEST(BufferLayoutTest, ElementSizeAndComponentCount) {
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float), 4u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float2), 8u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float3), 12u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float4), 16u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Mat4), 64u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Int), 4u);

  BufferElement elem(ShaderDataType::Float3, "a_Position");
  EXPECT_EQ(elem.GetComponentCount(), 3u);
  EXPECT_EQ(elem.size, 12u);
}

TEST(BufferLayoutTest, LayoutComputesCorrectStrideAndOffsets) {
  BufferLayout layout({
      {ShaderDataType::Float3, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
  });

  EXPECT_EQ(layout.GetStride(), 36u);  // 12 + 16 + 8 = 36 bytes

  const auto& elements = layout.GetElements();
  ASSERT_EQ(elements.size(), 3u);

  EXPECT_EQ(elements[0].offset, 0u);
  EXPECT_EQ(elements[0].size, 12u);

  EXPECT_EQ(elements[1].offset, 12u);
  EXPECT_EQ(elements[1].size, 16u);

  EXPECT_EQ(elements[2].offset, 28u);
  EXPECT_EQ(elements[2].size, 8u);
}

TEST(PrimitiveRendererTest, StatisticsResetAndAccumulation) {
  PrimitiveRenderer::ResetStats();
  const auto& stats = PrimitiveRenderer::GetStats();
  EXPECT_EQ(stats.draw_calls, 0u);
  EXPECT_EQ(stats.quad_count, 0u);
  EXPECT_EQ(stats.triangle_count, 0u);
  EXPECT_EQ(stats.line_count, 0u);
}

}  // namespace
