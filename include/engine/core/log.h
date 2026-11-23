#ifndef ENGINE_CORE_LOG_H_
#define ENGINE_CORE_LOG_H_

#include <iostream>
#include <string_view>

namespace engine::core {

enum class LogLevel {
  kTrace,
  kInfo,
  kWarn,
  kError,
  kFatal
};

class Log {
 public:
  static void Initialize();
  static void SetLevel(LogLevel level);
  static LogLevel GetLevel();

  static void Write(LogLevel level, std::string_view message);

 private:
  static inline LogLevel s_level_ = LogLevel::kInfo;
};

}  // namespace engine::core

#define ENGINE_LOG_TRACE(msg) ::engine::core::Log::Write(::engine::core::LogLevel::kTrace, msg)
#define ENGINE_LOG_INFO(msg)  ::engine::core::Log::Write(::engine::core::LogLevel::kInfo, msg)
#define ENGINE_LOG_WARN(msg)  ::engine::core::Log::Write(::engine::core::LogLevel::kWarn, msg)
#define ENGINE_LOG_ERROR(msg) ::engine::core::Log::Write(::engine::core::LogLevel::kError, msg)
#define ENGINE_LOG_FATAL(msg) ::engine::core::Log::Write(::engine::core::LogLevel::kFatal, msg)

#endif  // ENGINE_CORE_LOG_H_
