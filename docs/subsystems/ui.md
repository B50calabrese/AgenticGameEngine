# UI Subsystem

The **UI** subsystem (`engine::ui`) provides lightweight immediate-mode 2D UI primitives, mouse hit-testing (hover, click, drag), and optional textured rendering on top of `PrimitiveRenderer` and `Font`.

---

## 1. Public API Surface

Public headers live in `include/engine/ui/`:

| Header | Class / Struct | Description |
| :--- | :--- | :--- |
| `rect.h` | `Rect` | Axis-aligned 2D rectangle (`x, y, width, height`) with `Contains(point)` and `Overlaps(other)` hit-testing helpers. |
| `ui_context.h` | `UIContext`, `ButtonStyle` | Immediate-mode UI frame context (`BeginFrame`, `EndFrame`), `Panel`, `Label`, `Button`, `IsHovered`, and `IsDragging`. |

---

## 2. Usage Example

```cpp
engine::ui::UIContext ui;
ui.BeginFrame(static_cast<float>(window->GetHeight()));

// Solid or textured panel
ui.Panel({40.0f, 60.0f, 380.0f, 420.0f},
         engine::renderer::Color(0.13f, 0.15f, 0.21f, 1.0f),
         engine::renderer::Color(0.35f, 0.40f, 0.52f, 1.0f), 2.0f);

// Interactive button (returns true on click)
if (ui.Button("play", {60.0f, 310.0f, 220.0f, 48.0f}, "Play", font)) {
  // Handle click
}

// Textured button
engine::ui::ButtonStyle card_style;
card_style.texture = card_texture;
ui.Button("card_btn", {60.0f, 220.0f, 120.0f, 160.0f}, "", font, card_style);

// Draggable element
engine::math::Vector2 delta;
if (ui.IsDragging("card", card_bounds, &delta)) {
  card_bounds.x += delta.x;
  card_bounds.y += delta.y;
}

ui.EndFrame();
```

---

## 3. Demo Validation

Run the interactive UI demo:
```bash
make ui-demo
```
For automated headless testing:
```bash
./build/demos/ui_demo --frames 10
```
