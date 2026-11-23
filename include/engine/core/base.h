#ifndef ENGINE_CORE_BASE_H_
#define ENGINE_CORE_BASE_H_

#include <cstdint>
#include <memory>
#include <utility>

// Platform Detection
#if defined(_WIN32) || defined(_WIN64)
#define ENGINE_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
#define ENGINE_PLATFORM_MACOS 1
#elif defined(__linux__)
#define ENGINE_PLATFORM_LINUX 1
#else
#error "Unsupported platform!"
#endif

// Debug break macro
#if defined(ENGINE_PLATFORM_WINDOWS)
#define ENGINE_DEBUGBREAK() __debugbreak()
#elif defined(ENGINE_PLATFORM_LINUX) || defined(ENGINE_PLATFORM_MACOS)
#include <csignal>
#define ENGINE_DEBUGBREAK() raise(SIGTRAP)
#else
#define ENGINE_DEBUGBREAK()
#endif

// Utility aliases
namespace engine {

using EntityID = std::uint32_t;
constexpr EntityID kNullEntity = 0;

}  // namespace engine

#endif  // ENGINE_CORE_BASE_H_
