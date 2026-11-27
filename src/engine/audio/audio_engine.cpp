#include "engine/audio/audio_engine.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "engine/core/log.h"

namespace engine::audio {

namespace {

struct AudioState {
  bool initialized = false;
  float master_volume = 1.0f;
  bool music_playing = false;
  std::string current_music_path;
  std::uint32_t next_handle_id = 1;
  std::unordered_map<std::uint32_t, std::string> loaded_sounds;
};

AudioState& GetState() {
  static AudioState state;
  return state;
}

}  // namespace

bool AudioEngine::Initialize() {
  auto& s = GetState();
  s.initialized = true;
  ENGINE_LOG_INFO("[AudioEngine] Initialized.");
  return true;
}

void AudioEngine::Shutdown() {
  auto& s = GetState();
  s.loaded_sounds.clear();
  s.music_playing = false;
  s.current_music_path.clear();
  s.initialized = false;
  ENGINE_LOG_INFO("[AudioEngine] Shut down.");
}

bool AudioEngine::IsInitialized() { return GetState().initialized; }

SoundHandle AudioEngine::LoadSound(const std::string& path) {
  auto& s = GetState();
  if (!s.initialized || path.empty()) {
    return SoundHandle{0};
  }
  const std::uint32_t id = s.next_handle_id++;
  s.loaded_sounds[id] = path;
  return SoundHandle{id};
}

void WriteBeepWav(const std::string& path, float freq_hz, float duration_sec,
                  float volume, bool fade_out = true) {
  const int sample_rate = 44100;
  const int num_samples = static_cast<int>(sample_rate * duration_sec);
  const int data_size = num_samples * 2;
  const int chunk_size = 36 + data_size;

  std::ofstream out(path, std::ios::binary);
  if (!out) {
    return;
  }
  auto write_u32 = [&](std::uint32_t v) {
    out.write(reinterpret_cast<const char*>(&v), 4);
  };
  auto write_u16 = [&](std::uint16_t v) {
    out.write(reinterpret_cast<const char*>(&v), 2);
  };

  out.write("RIFF", 4);
  write_u32(static_cast<std::uint32_t>(chunk_size));
  out.write("WAVE", 4);
  out.write("fmt ", 4);
  write_u32(16);
  write_u16(1);
  write_u16(1);
  write_u32(static_cast<std::uint32_t>(sample_rate));
  write_u32(static_cast<std::uint32_t>(sample_rate * 2));
  write_u16(2);
  write_u16(16);
  out.write("data", 4);
  write_u32(static_cast<std::uint32_t>(data_size));

  const float amp = std::clamp(volume, 0.0f, 1.0f) * 24000.0f;
  for (int i = 0; i < num_samples; ++i) {
    const float t = static_cast<float>(i) / static_cast<float>(sample_rate);
    const float env = fade_out
                          ? (1.0f - (static_cast<float>(i) /
                                     static_cast<float>(num_samples)))
                          : 1.0f;
    const std::int16_t sample = static_cast<std::int16_t>(
        std::sin(2.0f * 3.14159265f * freq_hz * t) * amp * env);
    out.write(reinterpret_cast<const char*>(&sample), 2);
  }
}

void AudioEngine::PlaySfx(SoundHandle handle, float volume) {
  auto& s = GetState();
  if (!s.initialized || !handle.IsValid()) {
    return;
  }
  auto it = s.loaded_sounds.find(handle.id);
  if (it == s.loaded_sounds.end()) {
    return;
  }
  std::string play_path = it->second;
  if (!std::filesystem::exists(play_path)) {
    play_path = "/tmp/age_sfx_" + std::to_string(handle.id) + ".wav";
    WriteBeepWav(play_path, 660.0f, 0.12f, s.master_volume * volume);
  }
  const std::string cmd = "aplay -q \"" + play_path + "\" >/dev/null 2>&1 &";
  std::system(cmd.c_str());
}

bool AudioEngine::PlayMusic(const std::string& path, bool loop, float volume) {
  auto& s = GetState();
  if (!s.initialized || path.empty()) {
    return false;
  }
  StopMusic();
  std::string play_path = path;
  if (!std::filesystem::exists(play_path)) {
    play_path = "/tmp/age_music.wav";
    WriteBeepWav(play_path, 440.0f, 3.0f, s.master_volume * volume * 0.8f, false);
  }
  (void)loop;
  const std::string cmd = "aplay -q \"" + play_path + "\" </dev/null >/dev/null 2>&1 &";
  (void)std::system(cmd.c_str());
  s.current_music_path = path;
  s.music_playing = true;
  return true;
}

void AudioEngine::StopMusic() {
  auto& s = GetState();
  if (s.music_playing) {
    (void)std::system("pkill -f \"aplay -q /tmp/age_music.wav\" >/dev/null 2>&1");
  }
  s.music_playing = false;
  s.current_music_path.clear();
}

bool AudioEngine::IsMusicPlaying() { return GetState().music_playing; }

void AudioEngine::SetMasterVolume(float volume) {
  GetState().master_volume = std::clamp(volume, 0.0f, 1.0f);
}

float AudioEngine::GetMasterVolume() { return GetState().master_volume; }

}  // namespace engine::audio
