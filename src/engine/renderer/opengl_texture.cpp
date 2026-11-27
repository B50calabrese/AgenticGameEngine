#include "engine/renderer/opengl_texture.h"

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
#include <glad/glad.h>
#endif

#include <stb_image.h>

#include "engine/core/assert.h"
#include "engine/core/log.h"
#include "engine/renderer/render_command.h"

namespace engine::renderer {

namespace {

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
GLenum TextureFormatToOpenGLInternal(TextureFormat format) {
  switch (format) {
    case TextureFormat::R8:
      return GL_R8;
    case TextureFormat::RGB8:
      return GL_RGB8;
    case TextureFormat::RGBA8:
      return GL_RGBA8;
    case TextureFormat::None:
      return GL_NONE;
  }
  return GL_NONE;
}

GLenum TextureFormatToOpenGLData(TextureFormat format) {
  switch (format) {
    case TextureFormat::R8:
      return GL_RED;
    case TextureFormat::RGB8:
      return GL_RGB;
    case TextureFormat::RGBA8:
      return GL_RGBA;
    case TextureFormat::None:
      return GL_NONE;
  }
  return GL_NONE;
}
#endif

std::uint32_t BytesPerPixel(TextureFormat format) {
  switch (format) {
    case TextureFormat::R8:
      return 1;
    case TextureFormat::RGB8:
      return 3;
    case TextureFormat::RGBA8:
      return 4;
    case TextureFormat::None:
      return 0;
  }
  return 0;
}

}  // namespace

std::shared_ptr<Texture2D> Texture2D::Create(
    const TextureSpecification& specification) {
  return std::make_shared<OpenGLTexture2D>(specification);
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path) {
  return std::make_shared<OpenGLTexture2D>(path);
}

OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
    : specification_(specification),
      width_(specification.width),
      height_(specification.height) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (!RenderCommand::IsInitialized()) {
    return;
  }

  glGenTextures(1, &renderer_id_);
  glBindTexture(GL_TEXTURE_2D, renderer_id_);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  specification_.generate_mips ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GLenum internal_format = TextureFormatToOpenGLInternal(specification_.format);
  GLenum data_format = TextureFormatToOpenGLData(specification_.format);
  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internal_format),
               static_cast<GLsizei>(width_), static_cast<GLsizei>(height_), 0,
               data_format, GL_UNSIGNED_BYTE, nullptr);

  is_loaded_ = true;
#endif
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  int width = 0;
  int height = 0;
  int channels = 0;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if (!data) {
    ENGINE_LOG_ERROR("Failed to load texture image: " + path);
    return;
  }

  width_ = static_cast<std::uint32_t>(width);
  height_ = static_cast<std::uint32_t>(height);
  specification_.width = width_;
  specification_.height = height_;

  if (channels == 4) {
    specification_.format = TextureFormat::RGBA8;
  } else if (channels == 3) {
    specification_.format = TextureFormat::RGB8;
  } else if (channels == 1) {
    specification_.format = TextureFormat::R8;
  } else {
    ENGINE_LOG_ERROR("Unsupported channel count (" + std::to_string(channels) +
                     ") in texture: " + path);
    stbi_image_free(data);
    return;
  }

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (RenderCommand::IsInitialized()) {
    glGenTextures(1, &renderer_id_);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum internal_format = TextureFormatToOpenGLInternal(specification_.format);
    GLenum data_format = TextureFormatToOpenGLData(specification_.format);
    glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internal_format),
                 static_cast<GLsizei>(width_), static_cast<GLsizei>(height_), 0,
                 data_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    is_loaded_ = true;
  }
#endif

  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glDeleteTextures(1, &renderer_id_);
  }
#endif
}

void OpenGLTexture2D::SetData(const void* data, std::uint32_t size) {
  std::uint32_t bpp = BytesPerPixel(specification_.format);
  ENGINE_ASSERT(size == width_ * height_ * bpp,
                "Texture2D::SetData size must match entire texture!");
  (void)bpp;

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (!RenderCommand::IsInitialized() || renderer_id_ == 0) {
    return;
  }

  glBindTexture(GL_TEXTURE_2D, renderer_id_);
  GLenum data_format = TextureFormatToOpenGLData(specification_.format);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, static_cast<GLsizei>(width_),
                  static_cast<GLsizei>(height_), data_format, GL_UNSIGNED_BYTE,
                  data);
  if (specification_.generate_mips) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
#else
  (void)data;
  (void)size;
#endif
}

void OpenGLTexture2D::Bind(std::uint32_t slot) const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, renderer_id_);
  }
#else
  (void)slot;
#endif
}

}  // namespace engine::renderer
