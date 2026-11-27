#ifndef ENGINE_RENDERER_FONT_H_
#define ENGINE_RENDERER_FONT_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "engine/math/vector2.h"
#include "engine/renderer/color.h"
#include "engine/renderer/texture.h"

// Forward declarations for stb_truetype structures
struct stbtt_fontinfo;
struct stbtt_packedchar;

namespace engine::renderer {

/**
 * @brief Simple TrueType font handling using stb_truetype.
 *
 * Loads a TrueType font file, packs a configurable range of glyphs into a texture
 * atlas, and provides utilities to render text via PrimitiveRenderer.
 *
 * The font is immutable after construction – all data is cached for fast per‑
 * frame text rendering.
 */
class Font {
 public:
  /**
   * @brief Load a font from a file and generate an atlas.
   * @param path Filesystem path to a .ttf file.
   * @param pixel_height Desired height of glyphs in pixels.
   * @param first_char First Unicode codepoint to include (default 32 – space).
   * @param char_count Number of consecutive codepoints to pack (default 96 – ASCII printable).
   * @return Shared pointer to Font instance, or nullptr on failure.
   */
  static std::shared_ptr<Font> Load(const std::string& path, int pixel_height,
                                    int first_char = 32, int char_count = 96);

  /**
   * @brief Render a string at a given position.
   * @param text UTF‑8 string (ASCII subset assumed for simplicity).
   * @param position Bottom‑left corner of the first glyph.
   * @param color Tint to apply.
   */
  void DrawString(const std::string& text,
                  const engine::math::Vector2& position,
                  const Color& color) const;

  const std::shared_ptr<Texture2D>& GetAtlasTexture() const { return atlas_texture_; }

 private:
  Font() = default;

  bool Initialize(const std::string& path, int pixel_height,
                  int first_char, int char_count);

  std::shared_ptr<Texture2D> atlas_texture_;
  // Mapping from character code to packed glyph data
  std::unordered_map<char, stbtt_packedchar> glyphs_;
  int atlas_width_ = 0;
  int atlas_height_ = 0;
  float pixel_height_ = 0.0f;
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_FONT_H_
