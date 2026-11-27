#include <gtest/gtest.h>

#include <cstdint>

#include "engine/renderer/texture.h"

namespace {

using engine::renderer::Texture2D;
using engine::renderer::TextureFormat;
using engine::renderer::TextureSpecification;

TEST(Texture2DTest, SpecificationDefaultsAreValid) {
  TextureSpecification spec;
  EXPECT_EQ(spec.width, 1u);
  EXPECT_EQ(spec.height, 1u);
  EXPECT_EQ(spec.format, TextureFormat::RGBA8);
  EXPECT_TRUE(spec.generate_mips);
}

TEST(Texture2DTest, CreateFromSpecificationInitializesDimensions) {
  TextureSpecification spec;
  spec.width = 64;
  spec.height = 32;
  spec.format = TextureFormat::RGBA8;

  auto texture = Texture2D::Create(spec);
  ASSERT_NE(texture, nullptr);
  EXPECT_EQ(texture->GetWidth(), 64u);
  EXPECT_EQ(texture->GetHeight(), 32u);
  EXPECT_EQ(texture->GetSpecification().format, TextureFormat::RGBA8);

  std::uint32_t white_pixel = 0xFFFFFFFF;
  TextureSpecification tiny_spec;
  tiny_spec.width = 1;
  tiny_spec.height = 1;
  auto tiny_tex = Texture2D::Create(tiny_spec);
  tiny_tex->SetData(&white_pixel, sizeof(white_pixel));
  tiny_tex->Bind(0);
}

TEST(Texture2DTest, MissingFileHandlesGracefully) {
  auto texture = Texture2D::Create("non_existent_texture_file_12345.png");
  ASSERT_NE(texture, nullptr);
  EXPECT_FALSE(texture->IsLoaded());
  EXPECT_EQ(texture->GetWidth(), 0u);
  EXPECT_EQ(texture->GetHeight(), 0u);
}

}  // namespace
