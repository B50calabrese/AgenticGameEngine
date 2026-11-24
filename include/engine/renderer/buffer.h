#ifndef ENGINE_RENDERER_BUFFER_H_
#define ENGINE_RENDERER_BUFFER_H_

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace engine::renderer {

/**
 * @brief High-level data types supported in shader vertex buffers.
 */
enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Mat3,
  Mat4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

/**
 * @brief Calculates the byte size of a given ShaderDataType.
 */
std::uint32_t ShaderDataTypeSize(ShaderDataType type);

/**
 * @brief Describes a single vertex attribute in a vertex buffer layout.
 */
struct BufferElement {
  std::string name;
  ShaderDataType type = ShaderDataType::None;
  std::uint32_t size = 0;
  std::size_t offset = 0;
  bool normalized = false;

  BufferElement() = default;

  BufferElement(ShaderDataType element_type, std::string element_name,
                bool is_normalized = false)
      : name(std::move(element_name)),
        type(element_type),
        size(ShaderDataTypeSize(element_type)),
        offset(0),
        normalized(is_normalized) {}

  std::uint32_t GetComponentCount() const;
};

/**
 * @brief Manages the sequence of elements and computed stride of a vertex buffer layout.
 */
class BufferLayout {
 public:
  BufferLayout() = default;

  BufferLayout(std::initializer_list<BufferElement> elements)
      : elements_(elements) {
    CalculateOffsetsAndStride();
  }

  explicit BufferLayout(std::vector<BufferElement> elements)
      : elements_(std::move(elements)) {
    CalculateOffsetsAndStride();
  }

  std::uint32_t GetStride() const { return stride_; }
  const std::vector<BufferElement>& GetElements() const { return elements_; }

  std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
  std::vector<BufferElement>::iterator end() { return elements_.end(); }
  std::vector<BufferElement>::const_iterator begin() const { return elements_.begin(); }
  std::vector<BufferElement>::const_iterator end() const { return elements_.end(); }

 private:
  void CalculateOffsetsAndStride() {
    std::size_t current_offset = 0;
    stride_ = 0;
    for (auto& element : elements_) {
      element.offset = current_offset;
      current_offset += element.size;
      stride_ += element.size;
    }
  }

  std::vector<BufferElement> elements_;
  std::uint32_t stride_ = 0;
};

/**
 * @brief Platform-independent GPU Vertex Buffer.
 */
class VertexBuffer {
 public:
  virtual ~VertexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void SetData(const void* data, std::uint32_t size) = 0;

  virtual const BufferLayout& GetLayout() const = 0;
  virtual void SetLayout(const BufferLayout& layout) = 0;

  /**
   * @brief Factory creating a dynamic VertexBuffer with preallocated capacity.
   */
  static std::shared_ptr<VertexBuffer> Create(std::uint32_t size);

  /**
   * @brief Factory creating a static VertexBuffer initialized with vertex data.
   */
  static std::shared_ptr<VertexBuffer> Create(const void* vertices, std::uint32_t size);
};

/**
 * @brief Platform-independent GPU Index / Element Buffer.
 */
class IndexBuffer {
 public:
  virtual ~IndexBuffer() = default;

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual std::uint32_t GetCount() const = 0;

  /**
   * @brief Factory creating an IndexBuffer initialized with index data.
   */
  static std::shared_ptr<IndexBuffer> Create(const std::uint32_t* indices,
                                             std::uint32_t count);
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_BUFFER_H_
