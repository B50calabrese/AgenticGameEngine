# Core Subsystem

The **Core Subsystem** provides foundational utilities, logging, version metadata, platform detection, and assertion macros utilized across all engine modules.

* **Status**: Active & Implemented
* **Public Headers**: `include/engine/core/`
* **Internal Source**: `src/engine/core/`
* **Tests**: `tests/core_test.cpp`

---

## 1. Modules

### A. Base Definitions (`base.h`)
* **Header**: [include/engine/core/base.h](file:///home/antonio/code/AgenticGameEngine/include/engine/core/base.h)
* **Purpose**:
  * Cross-platform detection macros (`ENGINE_PLATFORM_LINUX`, `ENGINE_PLATFORM_WINDOWS`, `ENGINE_PLATFORM_MACOS`).
  * Debug break macro (`ENGINE_DEBUGBREAK()`) mapped to platform intrinsics (`raise(SIGTRAP)` on POSIX, `__debugbreak()` on MSVC).
  * Fundamental aliases (`EntityID`, `kNullEntity`).

### B. Semantic Versioning (`version.h`)
* **Header**: [include/engine/core/version.h](file:///home/antonio/code/AgenticGameEngine/include/engine/core/version.h)
* **Implementation**: [src/engine/core/version.cpp](file:///home/antonio/code/AgenticGameEngine/src/engine/core/version.cpp)
* **API**:
  ```cpp
  engine::Version GetVersion();        // Returns struct { major, minor, patch }
  std::string GetVersionString();      // Returns formatted string (e.g. "0.1.0")
  ```

### C. Logging System (`log.h`)
* **Header**: [include/engine/core/log.h](file:///home/antonio/code/AgenticGameEngine/include/engine/core/log.h)
* **Implementation**: [src/engine/core/log.cpp](file:///home/antonio/code/AgenticGameEngine/src/engine/core/log.cpp)
* **API & Macros**:
  ```cpp
  engine::core::Log::Initialize();
  engine::core::Log::SetLevel(engine::core::LogLevel::kWarn);

  ENGINE_LOG_TRACE("Detailed trace output");
  ENGINE_LOG_INFO("Informational message");
  ENGINE_LOG_WARN("Warning condition");
  ENGINE_LOG_ERROR("Recoverable error");
  ENGINE_LOG_FATAL("Fatal engine error");
  ```

### D. Assertion Macros (`assert.h`)
* **Header**: [include/engine/core/assert.h](file:///home/antonio/code/AgenticGameEngine/include/engine/core/assert.h)
* **Behavior**:
  * Active in debug builds (`!defined(NDEBUG)`).
  * Evaluates condition; on failure, prints diagnostic information to `std::cerr` (file, line, condition, message) and triggers `ENGINE_DEBUGBREAK()`.
  * Stripped to no-op in Release builds (`-DNDEBUG`).
* **Usage**:
  ```cpp
  ENGINE_ASSERT(ptr != nullptr, "Pointer must not be null during initialization!");
  ```

---

## 2. Invariants & Rules

1. **Zero External Dependencies**: The Core layer must only depend on the C++ Standard Library. It must never link to OpenGL, GLFW, or third-party libraries.
2. **Deterministic Startup**: `engine::core::Log::Initialize()` must be invoked before any subsystem logs messages.
