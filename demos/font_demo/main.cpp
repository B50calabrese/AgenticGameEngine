#include <iostream>
#include "engine/renderer/font.h"
#include "engine/renderer/primitive_renderer.h"
#include "engine/renderer/render_queue.h"
#include "engine/math/matrix4.h"

int main(int argc, char** argv) {
  // Simple headless demo: load a font and render a string.
  const std::string font_path = "thirdparty/fonts/Roboto-Regular.ttf";
  auto font = engine::renderer::Font::Load(font_path, 32);
  if (!font) {
    std::cerr << "Failed to load font: " << font_path << std::endl;
    return 1;
  }
  std::cout << "Loaded font from " << font_path << " (atlas size "
            << font->GetAtlasTexture()->GetWidth() << "x"
            << font->GetAtlasTexture()->GetHeight() << ")" << std::endl;

  // Create a simple orthographic projection (0,800) x (0,600).
  engine::math::Matrix4 proj = engine::math::Matrix4::Orthographic(0.0f, 800.0f, 0.0f, 600.0f);
  // Use RenderQueue to draw text.
  engine::renderer::RenderQueue rq;
  rq.SubmitTexturedQuad({100.0f, 300.0f}, {400.0f, 50.0f}, font->GetAtlasTexture(), {}, 0.0f);
  // Usually we would call font->DrawString here, but for this simple demo we just ensure no crashes.
  rq.Flush(proj);
  std::cout << "Demo completed successfully." << std::endl;
  return 0;
}
