#ifndef ENGINE_RENDERER_RENDER_COMMAND_H_
#define ENGINE_RENDERER_RENDER_COMMAND_H_

#include <cstdint>
#include <memory>

#include "engine/renderer/color.h"
#include "engine/renderer/vertex_array.h"

namespace engine::renderer {

/**
 * @brief Low-level rendering execution API wrapping hardware graphics context commands.
 */
class RenderCommand {
 public:
  /**
   * @brief Initializes the low-level rendering context and function pointer loader (GLAD).
   * @return True if initialized successfully, false otherwise.
   */
  static bool Initialize();

  /**
   * @brief Sets the rendering viewport dimensions.
   */
  static void SetViewport(std::uint32_t x, std::uint32_t y,
                          std::uint32_t width, std::uint32_t height);

  /**
   * @brief Sets the clear color for the frame buffer.
   */
  static void SetClearColor(const Color& color);

  /**
   * @brief Clears the color and depth buffers.
   */
  static void Clear();

  /**
   * @brief Executes indexed drawing on the bound vertex array.
   * @param vertex_array The VAO containing vertex and index buffers.
   * @param index_count Number of indices to draw (0 = use entire index buffer count).
   */
  static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array,
                          std::uint32_t index_count = 0);

  /**
   * @brief Executes line strip/list drawing on the bound vertex array.
   * @param vertex_array The VAO containing vertex buffers.
   * @param vertex_count Number of vertices to draw as lines.
   */
  static void DrawLines(const std::shared_ptr<VertexArray>& vertex_array,
                        std::uint32_t vertex_count);

  /**
   * @brief Sets line width for subsequent line render commands.
   */
  static void SetLineWidth(float width);

  /**
   * @brief Checks if the graphics context has been successfully initialized.
   */
  static bool IsInitialized();
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_RENDER_COMMAND_H_
