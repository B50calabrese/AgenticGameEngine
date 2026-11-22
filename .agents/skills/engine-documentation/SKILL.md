---
name: engine-documentation
description: Documentation standards, Architecture Decision Records (ADR) authoring, Doxygen API commenting, and submodule consumer guide maintenance.
---

# Engine Documentation Standards Skill

This skill guides the **Documentation Agent** and developers in authoring and maintaining high-quality documentation across the codebase.

---

## 1. Documentation Pillars

Our documentation serves three key audiences:
1. **Downstream Game Developers (Submodule Consumers)**: Need clear instructions on embedding the engine, configuring CMake, and using public APIs.
2. **Engine Contributors & Subagents**: Need clear architectural contracts, subsystem boundaries, and ADR rationales.
3. **API Users**: Need precise Doxygen comments in public headers explaining inputs, outputs, ownership, and failure modes.

---

## 2. Submodule Consumer Documentation Standard

Every subsystem and public API must document how external projects interact with it.
The root `README.md` and `docs/submodule_guide.md` must explain:
1. **Adding Submodule**:
   ```bash
   git submodule add <repo-url> third_party/engine
   ```
2. **CMake Integration**:
   ```cmake
   add_subdirectory(third_party/engine)
   target_link_libraries(MyGame PRIVATE engine)
   ```
3. **Minimal Working Example**: A complete, compilable ~20-line `main.cpp` initializing the engine window and running a tick.

---

## 3. Architecture Decision Records (ADR) Authoring

Whenever a foundational design choice is made (e.g., choice of ECS layout, OpenGL batching strategy, Lua binding mechanism, platform abstraction design):
1. Create `docs/adr/XXXX-<title-in-kebab-case>.md` using `docs/adr/template.md`.
2. Clearly articulate:
   * **Context**: What problem or constraint led to this decision?
   * **Considered Options**: At least 2 viable options with pros and cons.
   * **Decision**: Which option was chosen and why?
   * **Consequences**: Both positive advantages and negative trade-offs.
3. Add the new record to the index in `docs/adr/README.md`.

---

## 4. Doxygen Public API Comments

All public headers (`include/engine/...`) must be documented using Doxygen-style blocks:

```cpp
/**
 * @brief Submits a quad to be drawn in the current frame batch.
 *
 * Draw commands are sorted by shader program, texture, and Z-layer
 * before rendering.
 *
 * @param position World space center position of the quad.
 * @param size Width and height of the quad.
 * @param texture Handle to the texture to bind, or nullptr for flat color.
 * @param color Tint color applied to the quad vertices.
 */
void DrawQuad(const Vector3& position, const Vector2& size,
              const Texture* texture, const Color& color);
```
