#include "engine/core/version.h"

namespace engine {

namespace {
constexpr std::uint32_t kVersionMajor = 0;
constexpr std::uint32_t kVersionMinor = 1;
constexpr std::uint32_t kVersionPatch = 0;
}  // namespace

Version GetVersion() {
  return Version{kVersionMajor, kVersionMinor, kVersionPatch};
}

std::string GetVersionString() {
  return std::to_string(kVersionMajor) + "." +
         std::to_string(kVersionMinor) + "." +
         std::to_string(kVersionPatch);
}

}  // namespace engine
