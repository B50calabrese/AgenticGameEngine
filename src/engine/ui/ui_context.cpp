#include "engine/ui/ui_context.h"

#include "engine/input/input.h"
#include "engine/input/mouse_button_codes.h"
#include "engine/renderer/primitive_renderer.h"

namespace engine::ui {

void UIContext::BeginFrame(float viewport_height) {
  const auto raw_pos = engine::input::Input::GetMousePosition();
  const engine::math::Vector2 ui_pos(raw_pos.x, viewport_height - raw_pos.y);
  const bool is_down = engine::input::Input::IsMouseButtonPressed(
      engine::input::MouseButton::Left);
  BeginFrame(ui_pos, is_down);
}

void UIContext::BeginFrame(const engine::math::Vector2& mouse_pos,
                           bool mouse_down) {
  prev_mouse_pos_ = mouse_pos_;
  prev_mouse_down_ = mouse_down_;
  mouse_pos_ = mouse_pos;
  mouse_down_ = mouse_down;
}

void UIContext::EndFrame() {
  if (!mouse_down_) {
    active_id_.clear();
  }
}

bool UIContext::IsHovered(const Rect& bounds) const {
  return bounds.Contains(mouse_pos_);
}

bool UIContext::IsDragging(const std::string& id, const Rect& bounds,
                           engine::math::Vector2* out_delta) {
  if (mouse_down_ && !prev_mouse_down_ && IsHovered(bounds)) {
    active_id_ = id;
  }
  if (active_id_ == id && mouse_down_) {
    if (out_delta != nullptr) {
      *out_delta = mouse_pos_ - prev_mouse_pos_;
    }
    return true;
  }
  return false;
}

void UIContext::Panel(const Rect& bounds,
                      const engine::renderer::Color& bg_color,
                      const engine::renderer::Color& border_color,
                      float border_thickness,
                      const std::shared_ptr<engine::renderer::Texture2D>& texture) const {
  if (texture) {
    engine::renderer::PrimitiveRenderer::DrawTexturedQuad(
        {bounds.x, bounds.y}, {bounds.width, bounds.height}, texture, bg_color);
  } else {
    engine::renderer::PrimitiveRenderer::DrawQuad(
        {bounds.x, bounds.y}, {bounds.width, bounds.height}, bg_color);
  }
  if (border_color.a > 0.0f && border_thickness > 0.0f) {
    engine::renderer::PrimitiveRenderer::DrawRectOutline(
        bounds.x, bounds.y, bounds.width, bounds.height, border_color,
        border_thickness);
  }
}

void UIContext::Label(const std::string& text,
                      const engine::math::Vector2& position,
                      const std::shared_ptr<engine::renderer::Font>& font,
                      const engine::renderer::Color& color) const {
  if (font) {
    font->DrawString(text, position, color);
  }
}

bool UIContext::Button(const std::string& id, const Rect& bounds,
                       const std::string& label,
                       const std::shared_ptr<engine::renderer::Font>& font,
                       const ButtonStyle& style) {
  const bool hovered = IsHovered(bounds);
  bool clicked = false;

  if (hovered && mouse_down_ && !prev_mouse_down_) {
    active_id_ = id;
  }

  if (!mouse_down_ && prev_mouse_down_ && active_id_ == id) {
    if (hovered) {
      clicked = true;
    }
    active_id_.clear();
  }

  engine::renderer::Color fill = style.normal_color;
  if (active_id_ == id && mouse_down_) {
    fill = style.active_color;
  } else if (hovered) {
    fill = style.hover_color;
  }

  Panel(bounds, fill, style.border_color, 1.5f, style.texture);
  if (font && !label.empty()) {
    const engine::math::Vector2 text_pos(
        bounds.x + 12.0f, bounds.y + bounds.height * 0.35f);
    font->DrawString(label, text_pos, style.text_color);
  }

  return clicked;
}

}  // namespace engine::ui
