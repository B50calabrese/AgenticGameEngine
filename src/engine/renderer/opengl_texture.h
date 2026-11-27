#ifndef ENGINE_RENDERER_OPENGL_TEXTURE_H_
#define ENGINE_RENDERER_OPENGL_TEXTURE_H_

#include <cstdint>
#include <string>

#include "engine/renderer/texture.h"

namespace engine::renderer {

class OpenGLTexture2D : public Texture2D {
 public:
  explicit OpenGLTexture2D(const TextureSpecification& specification);
  explicit OpenGLTexture2D(const std::string& path);
  ~OpenGLTexture2D() override;

  const TextureSpecification& GetSpecification() const override {
    return specification_;
  }

  std::uint32_t GetWidth() const override { return width_; }
  std::uint32_t GetHeight() const override { return height_; }
  std::uint32_t GetRendererId() const override { return renderer_id_; }
  const std::string& GetPath() const override { return path_; }

  void SetData(const void* data, std::uint32_t size) override;
  void Bind(std::uint32_t slot = 0) const override;

  bool IsLoaded() const override { return is_loaded_; }

  bool operator==(const Texture2D& other) const override {
    return renderer_id_ == other.GetRendererId();
  }

 private:
  TextureSpecification specification_;
  std::string path_;
  std::uint32_t width_ = 0;
  std::uint32_t height_ = 0;
  std::uint32_t renderer_id_ = 0;
  bool is_loaded_ = false;
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_OPENGL_TEXTURE_H_
