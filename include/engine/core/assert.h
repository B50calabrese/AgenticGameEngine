#ifndef ENGINE_CORE_ASSERT_H_
#define ENGINE_CORE_ASSERT_H_

#include <iostream>

#include "engine/core/base.h"

#if !defined(NDEBUG)
#define ENGINE_ENABLE_ASSERTS 1
#endif

#if defined(ENGINE_ENABLE_ASSERTS)
#define ENGINE_ASSERT(condition, message)                                         \
  do {                                                                            \
    if (!(condition)) {                                                           \
      std::cerr << "[ASSERTION FAILED] " << message << "\n"                       \
                << "  File: " << __FILE__ << ":" << __LINE__ << "\n"              \
                << "  Condition: " << #condition << std::endl;                   \
      ENGINE_DEBUGBREAK();                                                        \
    }                                                                             \
  } while (false)
#else
#define ENGINE_ASSERT(condition, message) do { (void)(condition); } while (false)
#endif

#endif  // ENGINE_CORE_ASSERT_H_
