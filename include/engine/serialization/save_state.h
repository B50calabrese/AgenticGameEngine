#ifndef ENGINE_SERIALIZATION_SAVE_STATE_H_
#define ENGINE_SERIALIZATION_SAVE_STATE_H_

#include <string>
#include <unordered_map>

namespace engine::serialization {

/**
 * @brief Lightweight key-value save state container with file and string
 *        serialization for game runs, settings, and scenes.
 */
class SaveState {
 public:
  void SetInt(const std::string& key, int value);
  void SetFloat(const std::string& key, float value);
  void SetBool(const std::string& key, bool value);
  void SetString(const std::string& key, const std::string& value);

  int GetInt(const std::string& key, int default_val = 0) const;
  float GetFloat(const std::string& key, float default_val = 0.0f) const;
  bool GetBool(const std::string& key, bool default_val = false) const;
  std::string GetString(const std::string& key,
                        const std::string& default_val = "") const;

  bool HasKey(const std::string& key) const;
  void Clear();

  std::string SerializeToString() const;
  bool DeserializeFromString(const std::string& data);

  bool SaveToFile(const std::string& path) const;
  bool LoadFromFile(const std::string& path);

 private:
  std::unordered_map<std::string, std::string> entries_;
};

}  // namespace engine::serialization

#endif  // ENGINE_SERIALIZATION_SAVE_STATE_H_
