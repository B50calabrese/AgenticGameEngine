# Input Subsystem

The **Input** subsystem provides a polling-based interface for querying keyboard and mouse states. Built on the platform window abstraction, it isolates downstream game logic from raw GLFW or platform-specific event callbacks while maintaining zero-overhead inline checks.

---

## 1. Design & Architecture

```text
┌────────────────────────────────────────────────────────┐
│                   Downstream Game                      │
│   Input::IsKeyPressed(KeyCode::Space)                  │
│   Input::GetMousePosition()                            │
└───────────────────────────┬────────────────────────────┘
                            │
┌───────────────────────────▼────────────────────────────┐
│                    Input Interface                     │
│               (engine::input::Input)                   │
└───────────────────────────┬────────────────────────────┘
                            │
┌───────────────────────────▼────────────────────────────┐
│                  Active Platform Window                │
│                   (Window / GlfwWindow)                │
└────────────────────────────────────────────────────────┘
```

When a `Window` is created (such as `GlfwWindow`), it automatically registers itself as the active target for `Input`. If no window is active or the engine is running in a headless environment, `Input` methods return safe fallback defaults (`false` or zero vectors) without crashing.

---

## 2. Public API Surface

Public headers live under `include/engine/input/`:

| Header | Type | Description |
| :--- | :--- | :--- |
| `input.h` | `Input` | Static interface for polling key states, mouse button states, and cursor positions. |
| `key_codes.h` | `KeyCode` | Enum class mapping standard keyboard keys (`Space`, `A`–`Z`, `D0`–`D9`, arrows, modifiers, function keys). |
| `mouse_button_codes.h`| `MouseButton` | Enum class mapping mouse buttons (`Left`, `Right`, `Middle`, `Button0`–`Button7`). |

Related Core Timing:
| Header | Type | Description |
| :--- | :--- | :--- |
| `timestep.h` | `Timestep` | Float wrapper representing frame delta time with `.GetSeconds()` and `.GetMilliseconds()`. |
| `clock.h` | `Clock` | Precision timer providing `Tick()` to calculate delta time and total elapsed running time. |

---

## 3. Usage Examples

### Polling Keyboard Input in Game Loop
```cpp
#include "engine/engine.h"

using engine::input::Input;
using engine::input::KeyCode;

void UpdatePlayer(engine::math::Vector2* position, float speed, engine::core::Timestep dt) {
  if (Input::IsKeyPressed(KeyCode::W) || Input::IsKeyPressed(KeyCode::Up)) {
    position->y += speed * dt;
  }
  if (Input::IsKeyPressed(KeyCode::S) || Input::IsKeyPressed(KeyCode::Down)) {
    position->y -= speed * dt;
  }
  if (Input::IsKeyPressed(KeyCode::A) || Input::IsKeyPressed(KeyCode::Left)) {
    position->x -= speed * dt;
  }
  if (Input::IsKeyPressed(KeyCode::D) || Input::IsKeyPressed(KeyCode::Right)) {
    position->x += speed * dt;
  }
}
```

### Polling Mouse Position & Buttons
```cpp
using engine::input::Input;
using engine::input::MouseButton;

if (Input::IsMouseButtonPressed(MouseButton::Left)) {
  engine::math::Vector2 mouse = Input::GetMousePosition();
  // Fire projectile or place block at mouse position
}
```
