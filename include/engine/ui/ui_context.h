#ifndef ENGINE_UI_UI_CONTEXT_H_
#define ENGINE_UI_UI_CONTEXT_H_

#include <memory>
#include <string>

#include "engine/math/vector2.h"
#include "engine/renderer/color.h"
#include "engine/renderer/font.h"
#include "engine/renderer/texture.h"
#include "engine/ui/rect.h"

namespace engine::ui {

struct ButtonStyle {
  engine::renderer::Color normal_color = engine::renderer::Color(0.22f, 0.25f, 0.32f, 1.0f);
  engine::renderer::Color hover_color = engine::renderer::Color(0.32f, 0.36f, 0.45f, 1.0f);
  engine::renderer::Color active_color = engine::renderer::Color(0.16f, 0.18f, 0.24f, 1.0f);
  engine::renderer::Color border_color = engine::renderer::Color(0.55f, 0.60f, 0.70f, 1.0f);
  engine::renderer::Color text_color = engine::renderer::Color::White();
  std::shared_ptr<engine::renderer::Texture2D> texture = nullptr;
};

/**
 * @brief Lightweight immediate-mode 2D UI context for panels, labels, buttons,
 *        and mouse hit-testing (hover, click, drag).
 */
class UIContext {
 public:
  /**
   * @brief Begins a UI frame by polling engine::input::Input.
   * @param viewport_height Used to convert top-left window mouse Y to bottom-left UI Y.
   */
  void BeginFrame(float viewport_height);

  /**
   * @brief Begins a UI frame with explicit mouse state (useful for headless tests).
   */
  void BeginFrame(const engine::math::Vector2& mouse_pos, bool mouse_down);

  void EndFrame();

  bool IsHovered(const Rect& bounds) const;
  bool IsDragging(const std::string& id, const Rect& bounds,
                  engine::math::Vector2* out_delta = nullptr);

  void Panel(const Rect& bounds, const engine::renderer::Color& bg_color,
             const engine::renderer::Color& border_color = engine::renderer::Color(0.0f, 0.0f, 0.0f, 0.0f),
             float border_thickness = 1.0f,
             const std::shared_ptr<engine::renderer::Texture2D>& texture = nullptr) const;

  void Label(const std::string& text, const engine::math::Vector2& position,
             const std::shared_ptr<engine::renderer::Font>& font,
             const engine::renderer::Color& color = engine::renderer::Color::White()) const;

  bool Button(const std::string& id, const Rect& bounds, const std::string& label,
              const std::shared_ptr<engine::renderer::Font>& font,
              const ButtonStyle& style = ButtonStyle{});

  const engine::math::Vector2& GetMousePosition() const { return mouse_pos_; }
  bool IsMouseDown() const { return mouse_down_; }

 private:
  engine::math::Vector2 mouse_pos_{0.0f, 0.0f};
  engine::math::Vector2 prev_mouse_pos_{0.0f, 0.0f};
  bool mouse_down_ = false;
  bool prev_mouse_down_ = false;
  std::string active_id_;
};

}  // namespace engine::ui

#endif  // ENGINE_UI_UI_CONTEXT_H_
