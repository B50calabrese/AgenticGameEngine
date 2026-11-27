#include "engine/renderer/font.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/primitive_renderer.h"

#include <cmath>
#include <fstream>
#include <vector>
#include <cstring>

// stb_truetype implementation
// #define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "stb_truetype.h"

namespace engine::renderer {

std::shared_ptr<Font> Font::Load(const std::string& path, int pixel_height,
                                 int first_char, int char_count) {
  auto font = std::shared_ptr<Font>(new Font());
  if (font->Initialize(path, pixel_height, first_char, char_count)) {
    return font;
  }
  return nullptr;
}

bool Font::Initialize(const std::string& path, int pixel_height,
                      int first_char, int char_count) {
  // Load file into memory
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    return false;
  }
  file.seekg(0, std::ios::end);
  size_t size = static_cast<size_t>(file.tellg());
  file.seekg(0, std::ios::beg);
  std::vector<unsigned char> font_data(size);
  file.read(reinterpret_cast<char*>(font_data.data()), size);
  if (!file) return false;

  // Init font info
  stbtt_fontinfo font_info;
  if (!stbtt_InitFont(&font_info, font_data.data(),
                      stbtt_GetFontOffsetForIndex(font_data.data(), 0))) {
    return false;
  }

  // Atlas size (fixed for now)
  atlas_width_ = 512;
  atlas_height_ = 512;
  pixel_height_ = static_cast<float>(pixel_height);

  // Allocate bitmap for packing (1 byte per pixel, alpha only)
  std::vector<unsigned char> bitmap(atlas_width_ * atlas_height_);
  std::memset(bitmap.data(), 0, bitmap.size());

  // Pack characters
  stbtt_pack_context pack_ctx;
  if (!stbtt_PackBegin(&pack_ctx, bitmap.data(), atlas_width_, atlas_height_, 0, 1, nullptr)) {
    return false;
  }
  stbtt_PackSetOversampling(&pack_ctx, 2, 2);

  std::vector<stbtt_packedchar> packed_chars(char_count);
  // Packed character range configuration using stbtt_pack_range fields
  stbtt_pack_range range;
  range.font_size = static_cast<float>(pixel_height);
  range.first_unicode_codepoint_in_range = static_cast<int>(first_char);
  range.array_of_unicode_codepoints = nullptr;
  range.num_chars = char_count;
  range.chardata_for_range = packed_chars.data();
  range.h_oversample = 0;
  range.v_oversample = 0;

  stbtt_PackFontRanges(&pack_ctx, font_data.data(), 0, &range, 1);
  stbtt_PackEnd(&pack_ctx);

  // Convert 1-channel alpha bitmap to RGBA texture data
  const size_t pixel_count = static_cast<size_t>(atlas_width_) * static_cast<size_t>(atlas_height_);
  std::vector<unsigned char> tex_data(pixel_count * 4);
  for (size_t i = 0; i < pixel_count; ++i) {
    unsigned char a = bitmap[i];
    tex_data[i * 4 + 0] = 255; // R
    tex_data[i * 4 + 1] = 255; // G
    tex_data[i * 4 + 2] = 255; // B
    tex_data[i * 4 + 3] = a;   // A
  }

  // Create the texture
  TextureSpecification spec;
  spec.width = static_cast<std::uint32_t>(atlas_width_);
  spec.height = static_cast<std::uint32_t>(atlas_height_);
  spec.format = TextureFormat::RGBA8;
  spec.generate_mips = false;
  atlas_texture_ = Texture2D::Create(spec);
  if (!atlas_texture_) return false;
  atlas_texture_->SetData(tex_data.data(), static_cast<std::uint32_t>(tex_data.size()));

  // Store glyph metrics
  for (int i = 0; i < char_count; ++i) {
    char c = static_cast<char>(first_char + i);
    const auto& pc = packed_chars[i];
    glyphs_[c] = {static_cast<float>(pc.x0), static_cast<float>(pc.y0),
                  static_cast<float>(pc.x1), static_cast<float>(pc.y1),
                  pc.xoff, pc.yoff, pc.xoff2, pc.yoff2, pc.xadvance};
  }

  return true;
}

void Font::DrawString(const std::string& text,
                      const engine::math::Vector2& position,
                      const Color& color,
                      float rotation_radians) const {
  float cursor_x = 0.0f;
  const float inv_w = 1.0f / static_cast<float>(atlas_width_);
  const float inv_h = 1.0f / static_cast<float>(atlas_height_);
  const float cos_r = std::cos(rotation_radians);
  const float sin_r = std::sin(rotation_radians);

  for (char c : text) {
    auto it = glyphs_.find(c);
    if (it == glyphs_.end()) {
      cursor_x += pixel_height_ * 0.5f;
      continue;
    }
    const GlyphMetrics& g = it->second;
    const float w = g.xoff2 - g.xoff;
    const float h = g.yoff2 - g.yoff;

    if (w > 0.0f && h > 0.0f) {
      const float local_x = cursor_x + g.xoff;
      const float local_y = -g.yoff2;
      const float rot_x = local_x * cos_r - local_y * sin_r;
      const float rot_y = local_x * sin_r + local_y * cos_r;
      const engine::math::Vector2 glyph_pos(position.x + rot_x,
                                            position.y + rot_y);
      const engine::math::Vector2 uv_min(g.x0 * inv_w, g.y1 * inv_h);
      const engine::math::Vector2 uv_max(g.x1 * inv_w, g.y0 * inv_h);
      PrimitiveRenderer::DrawTexturedQuad(glyph_pos, {w, h}, atlas_texture_,
                                          uv_min, uv_max, rotation_radians,
                                          color);
    }
    cursor_x += g.xadvance;
  }
}

} // namespace engine::renderer
