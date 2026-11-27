#include <gtest/gtest.h>

#include "engine/audio/audio_engine.h"
#include "engine/serialization/save_state.h"

namespace {

TEST(AudioEngineTest, InitializeLoadAndMusicLifecycle) {
  EXPECT_TRUE(engine::audio::AudioEngine::Initialize());
  EXPECT_TRUE(engine::audio::AudioEngine::IsInitialized());

  engine::audio::AudioEngine::SetMasterVolume(0.75f);
  EXPECT_FLOAT_EQ(engine::audio::AudioEngine::GetMasterVolume(), 0.75f);

  auto sfx = engine::audio::AudioEngine::LoadSound("sfx/click.wav");
  EXPECT_TRUE(sfx.IsValid());
  engine::audio::AudioEngine::PlaySfx(sfx, 0.8f);

  EXPECT_TRUE(engine::audio::AudioEngine::PlayMusic("music/theme.ogg"));
  EXPECT_TRUE(engine::audio::AudioEngine::IsMusicPlaying());
  engine::audio::AudioEngine::StopMusic();
  EXPECT_FALSE(engine::audio::AudioEngine::IsMusicPlaying());

  engine::audio::AudioEngine::Shutdown();
  EXPECT_FALSE(engine::audio::AudioEngine::IsInitialized());
}

TEST(SaveStateTest, RoundTripStringSerialization) {
  engine::serialization::SaveState save;
  save.SetInt("gold", 250);
  save.SetFloat("volume", 0.6f);
  save.SetBool("muted", false);
  save.SetString("hero", "Ironclad");

  const std::string serialized = save.SerializeToString();

  engine::serialization::SaveState loaded;
  EXPECT_TRUE(loaded.DeserializeFromString(serialized));
  EXPECT_EQ(loaded.GetInt("gold"), 250);
  EXPECT_NEAR(loaded.GetFloat("volume"), 0.6f, 0.001f);
  EXPECT_FALSE(loaded.GetBool("muted"));
  EXPECT_EQ(loaded.GetString("hero"), "Ironclad");
}

}  // namespace
