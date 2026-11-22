---
name: google-cpp-style
description: Comprehensive Google C++ Style Guide rules, formatting standards, naming conventions, and memory safety mandates for engine development.
---

# Google C++ Style Guide for Game Engine Development

All C++ source and header files must conform to Google's C++ best practices.

---

## 1. Naming Conventions

| Entity | Convention | Example |
| :--- | :--- | :--- |
| **Types / Classes / Structs** | `PascalCase` | `class RenderQueue;`, `struct TransformComponent;` |
| **Functions / Methods** | `PascalCase` | `void Initialize();`, `EntityID CreateEntity();` |
| **Variables / Locals** | `snake_case` | `float delta_time = 0.016f;`, `size_t entity_count;` |
| **Parameters** | `snake_case` | `void SetPosition(const Vector3& position);` |
| **Private / Protected Members** | `snake_case_` (trailing underscore) | `std::vector<EntityID> active_entities_;` |
| **Constants & Enum Values** | `kPascalCase` | `constexpr size_t kMaxBatchQuads = 10000;` |
| **Namespaces** | `snake_case` | `namespace engine::ecs { ... }` |
| **Source / Header Files** | `snake_case` | `batch_renderer.h`, `transform_component.cpp` |
| **Directories** | Pure lowercase alphabet (no underscores/hyphens) | `include/engine/renderer/`, `src/engine/ecs/` |

---

## 2. Formatting & Control Flow

1. **Always Use Braces**:
   Never omit curly braces on single-line control structures:
   ```cpp
   // CORRECT:
   if (!is_initialized_) {
     return false;
   }

   // FORBIDDEN:
   if (!is_initialized_) return false;
   ```
2. **Column Limit & Indentation**:
   * Indent by 2 spaces (no tabs).
   * Column limit: 80-100 characters.
3. **Automated Formatting**:
   * Format all changes using `.clang-format`:
     ```bash
     find include src tests demos -name "*.h" -o -name "*.cpp" | xargs clang-format -i
     ```

---

## 3. Function Parameters & Output Semantics

* **Input parameters**: Pass value types (`int`, `float`, `enum`) by value. Pass objects by `const T&`.
* **Output parameters**: Use non-const pointers (`T*`), never non-const references (`T&`):
  ```cpp
  // CORRECT:
  bool QueryIntersection(const Ray& ray, HitResult* out_hit);

  // FORBIDDEN:
  bool QueryIntersection(const Ray& ray, HitResult& out_hit);
  ```
* Callers can immediately see at the call site that an argument may be mutated:
  `QueryIntersection(ray, &hit_result);`

---

## 4. Header Files & Namespaces

1. **Include Guards**:
   Use standard path-based `#ifndef` / `#define` / `#endif` or `#pragma once`.
2. **Order of Includes**:
   * Related header (`#include "engine/renderer/batch_renderer.h"`)
   * C system headers (`<cstdint>`, `<cmath>`)
   * C++ standard library headers (`<vector>`, `<memory>`, `<string>`)
   * Third-party library headers (`<glad/glad.h>`, `<GLFW/glfw3.h>`)
   * Project internal headers (`#include "engine/core/log.h"`)
3. **No `using namespace` in Headers**:
   Never write `using namespace std;` or `using namespace engine;` in header files. Use explicit scoping (`std::string`, `engine::ecs::Registry`).

---

## 5. Memory Management & RAII

* Every resource (OpenGL texture ID, shader program, window handle, file handle) must be encapsulated in an RAII wrapper with clear move semantics and deleted copy constructors if unique.
* Use `std::unique_ptr` for exclusive ownership.
* Avoid raw owning pointers (`new` / `delete`).
