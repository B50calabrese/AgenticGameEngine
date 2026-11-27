# Scripting Subsystem (Lua)

The **Scripting** subsystem (`engine::scripting`) embeds **Lua 5.4** behind an RAII C++ wrapper (`LuaState`) with zero Lua header leakage into downstream consumers.

---

## 1. Public API Surface

Public headers live in `include/engine/scripting/`:

| Header | Class | Description |
| :--- | :--- | :--- |
| `lua_state.h` | `LuaState` | RAII Lua 5.4 VM wrapper supporting `ExecuteString`, `ExecuteFile`, `CallFunction`, `RegisterEngineBindings`, and global getters. |

---

## 2. Engine Bindings (`engine.*`)

Calling `lua.RegisterEngineBindings()` exposes the `engine` table to Lua scripts:
- `engine.log(msg)` — Logs via `ENGINE_INFO`.
- `engine.draw_quad(x, y, w, h, r, g, b, a)` — Submits a colored quad to `PrimitiveRenderer`.
- `engine.draw_circle(x, y, radius, r, g, b, a)` — Submits a colored circle to `PrimitiveRenderer`.
- `engine.is_key_pressed(key_code)` — Polls `engine::input::Input::IsKeyPressed`.

---

## 3. Usage Example

```cpp
engine::scripting::LuaState lua;
lua.RegisterEngineBindings();

std::string error;
if (!lua.ExecuteString("engine.log('Hello from Lua!')", &error)) {
  ENGINE_ERROR("Lua error: {}", error);
}
```
