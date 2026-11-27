#ifndef ENGINE_RENDERER_FONT_H_
#define ENGINE_RENDERER_FONT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "engine/math/vector2.h"
#include "engine/renderer/color.h"
#include "engine/renderer/texture.h"

// No third-party headers in public API!

namespace engine::renderer {

struct GlyphMetrics {
  float x0 = 0.0f;
  float y0 = 0.0f;
  float x1 = 0.0f;
  float y1 = 0.0f;
  float xoff = 0.0f;
  float yoff = 0.0f;
  float xoff2 = 0.0f;
  float yoff2 = 0.0f;
  float xadvance = 0.0f;
};

/**
 * @brief Simple TrueType font handling using stb_truetype.
 *
 * This minimal implementation loads a TTF file and creates a 1x1 white texture
 * as a placeholder atlas. Glyph packing is not performed for the demo – the
 * class provides the required API surface for compilation and a stub for future
 * development.
 */
class Font {
 public:
  /**
   * @brief Load a font from a file and generate an atlas.
   * @param path Filesystem path to a .ttf file.
   * @param pixel_height Desired glyph height in pixels (stored for future use).
   * @param first_char First Unicode codepoint to include (default space).
   * @param char_count Number of consecutive codepoints to pack (default 96).
   * @return Shared pointer to Font instance, or nullptr on failure.
   */
  static std::shared_ptr<Font> Load(const std::string& path, int pixel_height,
                                    int first_char = 32, int char_count = 96);

  /**
   * @brief Render a string at a given position.
   * @param text UTF‑8 string (ASCII subset assumed).
   * @param position Bottom‑left corner of the first glyph.
   * @param color Tint to apply.
   */
  void DrawString(const std::string& text,
                  const engine::math::Vector2& position,
                  const Color& color,
                  float rotation_radians = 0.0f) const;

  const std::shared_ptr<Texture2D>& GetAtlasTexture() const { return atlas_texture_; }

 private:
  // Constructor can be used via std::make_shared; keep it public for simplicity.
  Font() = default;

  bool Initialize(const std::string& path, int pixel_height,
                  int first_char, int char_count);

  std::shared_ptr<Texture2D> atlas_texture_;
  std::unordered_map<char, GlyphMetrics> glyphs_;
  int atlas_width_ = 0;
  int atlas_height_ = 0;
  float pixel_height_ = 0.0f;
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_FONT_H_