#ifndef ENGINE_RENDERER_PRIMITIVE_RENDERER_H_
#define ENGINE_RENDERER_PRIMITIVE_RENDERER_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "engine/renderer/color.h"

namespace engine::renderer {

/**
 * @brief High-level batched 2D geometric primitive renderer.
 *
 * Supports fast rendering of colored quads, rotated quads, triangles,
 * lines, rectangles, and circles with automatic batching and zero per-frame heap allocations.
 */
class PrimitiveRenderer {
 public:
  struct Statistics {
    std::uint32_t draw_calls = 0;
    std::uint32_t quad_count = 0;
    std::uint32_t triangle_count = 0;
    std::uint32_t line_count = 0;

    std::uint32_t GetTotalVertexCount() const {
      return (quad_count * 4) + (triangle_count * 3) + (line_count * 2);
    }
  };

  /**
   * @brief Initializes primitive renderer resources (buffers, shaders).
   */
  static bool Initialize();

  /**
   * @brief Frees GPU resources allocated by the primitive renderer.
   */
  static void Shutdown();

  /**
   * @brief Begins a new 2D primitive rendering batch with an orthographic projection.
   *
   * @param view_width Viewport width in screen units.
   * @param view_height Viewport height in screen units.
   * @param y_down If true, (0,0) is top-left with Y pointing down; if false, (0,0) is bottom-left.
   */
  static void BeginBatch(float view_width, float view_height, bool y_down = false);

  /**
   * @brief Begins a batch using an explicit 4x4 column-major projection matrix.
   *
   * @param projection_matrix Pointer to 16 contiguous float values.
   */
  static void BeginBatch(const float* projection_matrix);

  /**
   * @brief Finishes and flushes the current batch.
   */
  static void EndBatch();

  /**
   * @brief Forces an immediate upload and draw of accumulated primitives.
   */
  static void Flush();

  // --- Geometric Drawing Methods ---

  /**
   * @brief Draws a solid colored axis-aligned rectangle (quad).
   */
  static void DrawQuad(float x, float y, float width, float height,
                       const Color& color);

  /**
   * @brief Draws a solid colored quad rotated around its center.
   *
   * @param x Center or origin X.
   * @param y Center or origin Y.
   * @param width Quad width.
   * @param height Quad height.
   * @param rotation_radians Angle in radians.
   * @param color Fill color.
   */
  static void DrawRotatedQuad(float x, float y, float width, float height,
                              float rotation_radians, const Color& color);

  /**
   * @brief Draws a solid colored triangle.
   */
  static void DrawTriangle(float x1, float y1, float x2, float y2,
                           float x3, float y3, const Color& color);

  /**
   * @brief Draws a line between two points.
   */
  static void DrawLine(float x1, float y1, float x2, float y2,
                       const Color& color, float line_width = 1.0f);

  /**
   * @brief Draws an unfilled rectangle outline using lines.
   */
  static void DrawRectOutline(float x, float y, float width, float height,
                              const Color& color, float line_width = 1.0f);

  /**
   * @brief Draws a filled or outlined regular circle.
   */
  static void DrawCircle(float center_x, float center_y, float radius,
                         const Color& color, std::uint32_t segments = 32);

  // --- Statistics ---
  static const Statistics& GetStats();
  static void ResetStats();

 private:
  static void FlushQuads();
  static void FlushLines();
  static void NextBatch();
};

}  // namespace engine::renderer

#endif  // ENGINE_RENDERER_PRIMITIVE_RENDERER_H_
