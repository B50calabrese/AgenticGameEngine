#include "engine/serialization/save_state.h"

#include <fstream>
#include <sstream>

namespace engine::serialization {

void SaveState::SetInt(const std::string& key, int value) {
  entries_[key] = std::to_string(value);
}

void SaveState::SetFloat(const std::string& key, float value) {
  entries_[key] = std::to_string(value);
}

void SaveState::SetBool(const std::string& key, bool value) {
  entries_[key] = value ? "true" : "false";
}

void SaveState::SetString(const std::string& key, const std::string& value) {
  entries_[key] = value;
}

int SaveState::GetInt(const std::string& key, int default_val) const {
  auto it = entries_.find(key);
  if (it == entries_.end()) {
    return default_val;
  }
  try {
    return std::stoi(it->second);
  } catch (...) {
    return default_val;
  }
}

float SaveState::GetFloat(const std::string& key, float default_val) const {
  auto it = entries_.find(key);
  if (it == entries_.end()) {
    return default_val;
  }
  try {
    return std::stof(it->second);
  } catch (...) {
    return default_val;
  }
}

bool SaveState::GetBool(const std::string& key, bool default_val) const {
  auto it = entries_.find(key);
  if (it == entries_.end()) {
    return default_val;
  }
  if (it->second == "true" || it->second == "1") {
    return true;
  }
  if (it->second == "false" || it->second == "0") {
    return false;
  }
  return default_val;
}

std::string SaveState::GetString(const std::string& key,
                                 const std::string& default_val) const {
  auto it = entries_.find(key);
  return it != entries_.end() ? it->second : default_val;
}

bool SaveState::HasKey(const std::string& key) const {
  return entries_.find(key) != entries_.end();
}

void SaveState::Clear() { entries_.clear(); }

std::string SaveState::SerializeToString() const {
  std::ostringstream out;
  for (const auto& [k, v] : entries_) {
    out << k << "=" << v << "\n";
  }
  return out.str();
}

bool SaveState::DeserializeFromString(const std::string& data) {
  entries_.clear();
  std::istringstream in(data);
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    const auto eq = line.find('=');
    if (eq == std::string::npos) {
      continue;
    }
    entries_[line.substr(0, eq)] = line.substr(eq + 1);
  }
  return true;
}

bool SaveState::SaveToFile(const std::string& path) const {
  std::ofstream file(path, std::ios::trunc);
  if (!file) {
    return false;
  }
  file << SerializeToString();
  return static_cast<bool>(file);
}

bool SaveState::LoadFromFile(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    return false;
  }
  std::ostringstream ss;
  ss << file.rdbuf();
  return DeserializeFromString(ss.str());
}

}  // namespace engine::serialization
