#include "engine/renderer/opengl_shader.h"

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
#include <glad/glad.h>
#endif

#include <fstream>
#include <sstream>
#include <vector>

#include "engine/core/assert.h"
#include "engine/core/log.h"

namespace engine::renderer {

std::shared_ptr<Shader> Shader::Create(const std::string& name,
                                       const std::string& vertex_src,
                                       const std::string& fragment_src) {
  return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);
}

std::shared_ptr<Shader> Shader::CreateFromFile(const std::string& filepath) {
  return std::make_shared<OpenGLShader>(filepath);
}

OpenGLShader::OpenGLShader(std::string name, const std::string& vertex_src,
                           const std::string& fragment_src)
    : name_(std::move(name)) {
  Compile(vertex_src, fragment_src);
}

OpenGLShader::OpenGLShader(const std::string& filepath) {
  std::ifstream stream(filepath);
  if (!stream.is_open()) {
    ENGINE_LOG_ERROR("Could not open shader file: " + filepath);
    return;
  }

  // Extract name from filepath
  auto last_slash = filepath.find_last_of("/\\");
  last_slash = (last_slash == std::string::npos) ? 0 : last_slash + 1;
  auto last_dot = filepath.rfind('.');
  auto count = (last_dot == std::string::npos) ? filepath.size() - last_slash
                                               : last_dot - last_slash;
  name_ = filepath.substr(last_slash, count);

  enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
  std::stringstream ss[2];
  std::string line;
  ShaderType type = ShaderType::NONE;

  while (std::getline(stream, line)) {
    if (line.find("#type") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::VERTEX;
      } else if (line.find("fragment") != std::string::npos ||
                 line.find("pixel") != std::string::npos) {
        type = ShaderType::FRAGMENT;
      }
    } else if (type != ShaderType::NONE) {
      ss[static_cast<int>(type)] << line << '\n';
    }
  }

  Compile(ss[0].str(), ss[1].str());
}

OpenGLShader::~OpenGLShader() {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glDeleteProgram(renderer_id_);
  }
#endif
}

void OpenGLShader::Compile(const std::string& vertex_src,
                           const std::string& fragment_src) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* v_source = vertex_src.c_str();
  glShaderSource(vertex_shader, 1, &v_source, nullptr);
  glCompileShader(vertex_shader);

  GLint is_compiled = 0;
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    GLint max_length = 0;
    glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);
    std::vector<GLchar> info_log(max_length);
    glGetShaderInfoLog(vertex_shader, max_length, &max_length, &info_log[0]);
    glDeleteShader(vertex_shader);
    ENGINE_LOG_ERROR("Vertex Shader compilation failed (" + name_ + "): " +
                     std::string(info_log.data()));
    return;
  }

  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* f_source = fragment_src.c_str();
  glShaderSource(fragment_shader, 1, &f_source, nullptr);
  glCompileShader(fragment_shader);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
  if (is_compiled == GL_FALSE) {
    GLint max_length = 0;
    glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);
    std::vector<GLchar> info_log(max_length);
    glGetShaderInfoLog(fragment_shader, max_length, &max_length, &info_log[0]);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
    ENGINE_LOG_ERROR("Fragment Shader compilation failed (" + name_ + "): " +
                     std::string(info_log.data()));
    return;
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  GLint is_linked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
  if (is_linked == GL_FALSE) {
    GLint max_length = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
    std::vector<GLchar> info_log(max_length);
    glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);
    glDeleteProgram(program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    ENGINE_LOG_ERROR("Shader program linking failed (" + name_ + "): " +
                     std::string(info_log.data()));
    return;
  }

  glDetachShader(program, vertex_shader);
  glDetachShader(program, fragment_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  renderer_id_ = program;
#else
  (void)vertex_src;
  (void)fragment_src;
#endif
}

void OpenGLShader::Bind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ != 0) {
    glUseProgram(renderer_id_);
  }
#endif
}

void OpenGLShader::Unbind() const {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  glUseProgram(0);
#endif
}

int OpenGLShader::GetUniformLocation(const std::string& name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }

#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  if (renderer_id_ == 0) {
    return -1;
  }
  int location = glGetUniformLocation(renderer_id_, name.c_str());
  if (location == -1) {
    ENGINE_LOG_WARN("Uniform '" + name + "' not found or not active in shader '" + name_ + "'");
  }
  uniform_location_cache_[name] = location;
  return location;
#else
  return -1;
#endif
}

void OpenGLShader::SetInt(const std::string& name, int value) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform1i(location, value);
  }
#else
  (void)name;
  (void)value;
#endif
}

void OpenGLShader::SetFloat(const std::string& name, float value) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform1f(location, value);
  }
#else
  (void)name;
  (void)value;
#endif
}

void OpenGLShader::SetFloat2(const std::string& name, float x, float y) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform2f(location, x, y);
  }
#else
  (void)name;
  (void)x;
  (void)y;
#endif
}

void OpenGLShader::SetFloat3(const std::string& name, float x, float y, float z) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform3f(location, x, y, z);
  }
#else
  (void)name;
  (void)x;
  (void)y;
  (void)z;
#endif
}

void OpenGLShader::SetFloat4(const std::string& name, float x, float y, float z,
                             float w) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniform4f(location, x, y, z, w);
  }
#else
  (void)name;
  (void)x;
  (void)y;
  (void)z;
  (void)w;
#endif
}

void OpenGLShader::SetMat4(const std::string& name, const float* values) {
#if defined(ENGINE_ENABLE_OPENGL) || defined(ENGINE_ENABLE_GLFW)
  int location = GetUniformLocation(name);
  if (location != -1) {
    glUniformMatrix4fv(location, 1, GL_FALSE, values);
  }
#else
  (void)name;
  (void)values;
#endif
}

}  // namespace engine::renderer
