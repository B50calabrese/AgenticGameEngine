#include <filesystem>
#include <gtest/gtest.h>

#include "engine/renderer/font.h"

namespace engine::renderer {
namespace {

TEST(FontTest, MissingFileReturnsNullptr) {
  auto font = Font::Load("nonexistent/path/missing.ttf", 24);
  EXPECT_EQ(font, nullptr);
}

TEST(FontTest, LoadValidFontCreatesAtlasTexture) {
  std::string font_path = "thirdparty/fonts/Quicksand-Regular.ttf";
  if (!std::filesystem::exists(font_path)) {
    font_path = "../../thirdparty/fonts/Quicksand-Regular.ttf";
  }
  auto font = Font::Load(font_path, 32);
  ASSERT_NE(font, nullptr);
  ASSERT_NE(font->GetAtlasTexture(), nullptr);
  EXPECT_EQ(font->GetAtlasTexture()->GetWidth(), 512u);
  EXPECT_EQ(font->GetAtlasTexture()->GetHeight(), 512u);
}

}  // namespace
}  // namespace engine::renderer
