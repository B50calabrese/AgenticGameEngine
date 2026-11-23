#include <gtest/gtest.h>

#include "engine/core/base.h"
#include "engine/core/log.h"
#include "engine/core/version.h"
#include "engine/engine.h"

TEST(CoreTest, VersionMatchesSemanticFormat) {
  auto version = engine::GetVersion();
  EXPECT_EQ(version.major, 0u);
  EXPECT_EQ(version.minor, 1u);
  EXPECT_EQ(version.patch, 0u);

  std::string version_string = engine::GetVersionString();
  EXPECT_EQ(version_string, "0.1.0");
}

TEST(CoreTest, LoggingLevelFilter) {
  engine::core::Log::Initialize();
  engine::core::Log::SetLevel(engine::core::LogLevel::kWarn);
  EXPECT_EQ(engine::core::Log::GetLevel(), engine::core::LogLevel::kWarn);
}

TEST(CoreTest, PlatformDetectionDefined) {
#if defined(ENGINE_PLATFORM_LINUX) || defined(ENGINE_PLATFORM_WINDOWS) || defined(ENGINE_PLATFORM_MACOS)
  SUCCEED();
#else
  FAIL() << "No valid platform detected!";
#endif
}
