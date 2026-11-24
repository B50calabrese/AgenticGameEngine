#ifndef ENGINE_RENDERER_SHADER_H_
#define ENGINE_RENDERER_SHADER_H_

#include <memory>
#include <string>

namespace engine::renderer {

/**
 * @brief Platform-independent GPU Shader Program abstraction.
 */
class Shader {
 public:
  virtual ~Shader() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetInt(const std::string& name, int value) = 0;
  virtual void SetFloat(const std::string& name, float value) = 0;
  virtual void SetFloat2(const std::string& name, float x, float y) = 0;
  virtual void SetFloat3(const std::string& name, float x, float y, float z) = 0;
  virtual void SetFloat4(const std::string& name, float x, float y, float z, float w) = 0;
  virtual void SetMat4(const std::string& name, const float* values) = 0;

  virtual const std::string& GetName() const = 0;

  /**
   * @brief Factory creating a Shader program compiled from vertex and fragment GLSL source strings.
   */
  static std::shared_ptr<Shader> Create(const std::string& name,
                                       const std::string& vertex_src,
                                       const std::string& fragment_src);

  /**
   * @brief Factory creating a Shader program loaded from a combined or separate GLSL file.
   */
  static std::shared_ptr<Shader> CreateFromFile(const std::string& filepath);
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_SHADER_H_
