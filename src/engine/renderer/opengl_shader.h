#ifndef SRC_ENGINE_RENDERER_OPENGL_SHADER_H_
#define SRC_ENGINE_RENDERER_OPENGL_SHADER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

#include "engine/renderer/shader.h"

namespace engine::renderer {

class OpenGLShader : public Shader {
 public:
  OpenGLShader(std::string name, const std::string& vertex_src,
               const std::string& fragment_src);
  explicit OpenGLShader(const std::string& filepath);
  ~OpenGLShader() override;

  void Bind() const override;
  void Unbind() const override;

  void SetInt(const std::string& name, int value) override;
  void SetFloat(const std::string& name, float value) override;
  void SetFloat2(const std::string& name, float x, float y) override;
  void SetFloat3(const std::string& name, float x, float y, float z) override;
  void SetFloat4(const std::string& name, float x, float y, float z, float w) override;
  void SetMat4(const std::string& name, const float* values) override;

  const std::string& GetName() const override { return name_; }
  std::uint32_t GetRendererId() const { return renderer_id_; }

 private:
  void Compile(const std::string& vertex_src, const std::string& fragment_src);
  int GetUniformLocation(const std::string& name);

  std::uint32_t renderer_id_ = 0;
  std::string name_;
  std::unordered_map<std::string, int> uniform_location_cache_;
};

}  // namespace engine::renderer

#endif  // SRC_ENGINE_RENDERER_OPENGL_SHADER_H_
