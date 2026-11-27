#ifndef ENGINE_RENDERER_TEXTURE_H_
#define ENGINE_RENDERER_TEXTURE_H_

#include <cstdint>
#include <memory>
#include <string>

namespace engine::renderer {

/**
 * @brief Supported pixel formats for GPU textures.
 */
enum class TextureFormat {
  None = 0,
  R8,
  RGB8,
  RGBA8
};

/**
 * @brief Configuration specification for creating a Texture2D from memory.
 */
struct TextureSpecification {
  std::uint32_t width = 1;
  std::uint32_t height = 1;
  TextureFormat format = TextureFormat::RGBA8;
  bool generate_mips = true;
};

/**
 * @brief Platform-independent 2D GPU Texture interface.
 */
class Texture2D {
 public:
  virtual ~Texture2D() = default;

  virtual const TextureSpecification& GetSpecification() const = 0;

  virtual std::uint32_t GetWidth() const = 0;
  virtual std::uint32_t GetHeight() const = 0;
  virtual std::uint32_t GetRendererId() const = 0;
  virtual const std::string& GetPath() const = 0;

  /**
   * @brief Uploads raw pixel data to the GPU texture.
   * @param data Pointer to pixel buffer.
   * @param size Size of pixel buffer in bytes.
   */
  virtual void SetData(const void* data, std::uint32_t size) = 0;

  /**
   * @brief Binds the texture to the specified texture unit slot.
   */
  virtual void Bind(std::uint32_t slot = 0) const = 0;

  virtual bool IsLoaded() const = 0;

  virtual bool operator==(const Texture2D& other) const = 0;

  /**
   * @brief Creates an empty or procedural Texture2D matching the given specification.
   */
  static std::shared_ptr<Texture2D> Create(const TextureSpecification& specification);

  /**
   * @brief Loads an image file from disk (PNG, JPG, BMP, TGA) into a GPU Texture2D.
   */
  static std::shared_ptr<Texture2D> Create(const std::string& path);
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_TEXTURE_H_
