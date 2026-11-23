# Google C++ Guidelines & Best Practices for Engine Development

All engine C++ code must strictly conform to the Google C++ Style Guide and the mandates established below.

---

## 1. Naming Standards

* **Classes, Structs, Enums, Type Aliases**: `PascalCase` (`class RenderQueue;`, `struct TransformComponent;`)
* **Functions & Methods**: `PascalCase` (`void Initialize();`, `void Update(float delta_time);`)
* **Variables, Parameters, Namespaces**: `snake_case` (`float delta_time;`, `namespace engine::ecs { ... }`)
* **Private / Protected Class Members**: `snake_case_` with trailing underscore (`std::vector<EntityID> active_entities_;`)
* **Constants & Enumerators**: `kPascalCase` (`constexpr size_t kMaxBatchQuads = 10000;`, `enum class State { kIdle, kRunning };`)
* **Macros**: `ALL_CAPS` (`ENGINE_ASSERT`, `ENGINE_LOG_INFO`)

---

## 2. Syntax & Formatting Mandates

1. **Braces Required Everywhere**:
   Never write single-line control blocks without braces.
   ```cpp
   // REQUIRED:
   if (condition) {
     DoSomething();
   }

   // FORBIDDEN:
   if (condition) DoSomething();
   ```
2. **Output Parameters**:
   Use non-const pointers (`T* out_param`) for parameters intended to be modified by the function. Use `const T&` for read-only object inputs.
3. **No `using namespace` in Headers**:
   `using namespace std;` or `using namespace engine;` in header files is strictly prohibited. Always use explicit qualifiers in headers.

---

## 3. Memory & Ownership Mandates

1. **Strict RAII**:
   Every raw OS handle, OpenGL texture ID, shader ID, or buffer must be wrapped in an RAII class that frees the resource in its destructor.
2. **Explicit Smart Pointers**:
   * Exclusive ownership: `std::unique_ptr`
   * Shared ownership: `std::shared_ptr` (use sparingly; avoid circular ownership)
   * Non-owning access: Raw observer pointer (`T*`) or `const T&` (never delete an observer pointer)
3. **Contiguous Storage for Systems**:
   Components and entities must be stored in contiguous arrays (`std::vector`) to maximize cache locality during updates. Avoid pointer-chasing across the heap.
