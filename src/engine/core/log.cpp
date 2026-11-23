#include "engine/core/log.h"

#include <chrono>
#include <iomanip>
#include <iostream>

namespace engine::core {

void Log::Initialize() {
  s_level_ = LogLevel::kInfo;
}

void Log::SetLevel(LogLevel level) {
  s_level_ = level;
}

LogLevel Log::GetLevel() {
  return s_level_;
}

void Log::Write(LogLevel level, std::string_view message) {
  if (static_cast<int>(level) < static_cast<int>(s_level_)) {
    return;
  }

  const char* tag = "[INFO]";
  switch (level) {
    case LogLevel::kTrace:
      tag = "[TRACE]";
      break;
    case LogLevel::kInfo:
      tag = "[INFO]";
      break;
    case LogLevel::kWarn:
      tag = "[WARN]";
      break;
    case LogLevel::kError:
      tag = "[ERROR]";
      break;
    case LogLevel::kFatal:
      tag = "[FATAL]";
      break;
  }

  std::cout << tag << " " << message << std::endl;
}

}  // namespace engine::core
