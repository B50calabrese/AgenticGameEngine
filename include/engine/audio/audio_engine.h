#ifndef ENGINE_AUDIO_AUDIO_ENGINE_H_
#define ENGINE_AUDIO_AUDIO_ENGINE_H_

#include <cstdint>
#include <string>

namespace engine::audio {

struct SoundHandle {
  std::uint32_t id = 0;
  bool IsValid() const { return id != 0; }
};

/**
 * @brief Platform-independent audio engine abstraction for SFX and music.
 */
class AudioEngine {
 public:
  static bool Initialize();
  static void Shutdown();
  static bool IsInitialized();

  static SoundHandle LoadSound(const std::string& path);
  static void PlaySfx(SoundHandle handle, float volume = 1.0f);

  static bool PlayMusic(const std::string& path, bool loop = true,
                        float volume = 1.0f);
  static void StopMusic();
  static bool IsMusicPlaying();

  static void SetMasterVolume(float volume);
  static float GetMasterVolume();
};

}  // namespace engine::audio

#endif  // ENGINE_AUDIO_AUDIO_ENGINE_H_
