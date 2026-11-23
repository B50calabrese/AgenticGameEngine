#ifndef ENGINE_CORE_VERSION_H_
#define ENGINE_CORE_VERSION_H_

#include <cstdint>
#include <string>

namespace engine {

struct Version {
  std::uint32_t major;
  std::uint32_t minor;
  std::uint32_t patch;
};

/**
 * @brief Returns the semantic version structure of the engine.
 */
Version GetVersion();

/**
 * @brief Returns the formatted version string (e.g., "0.1.0").
 */
std::string GetVersionString();

}  // namespace engine

#endif  // ENGINE_CORE_VERSION_H_
