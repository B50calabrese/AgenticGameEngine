# Audio & Save State Subsystems

## 1. Audio (`engine::audio::AudioEngine`)
Provides `Initialize`, `Shutdown`, `LoadSound`, `PlaySfx`, `PlayMusic`, `StopMusic`, and `SetMasterVolume` in `include/engine/audio/audio_engine.h`.

## 2. Serialization (`engine::serialization::SaveState`)
Provides a key-value `SaveState` container in `include/engine/serialization/save_state.h` supporting `SetInt`/`GetInt`, `SetFloat`/`GetFloat`, `SetBool`/`GetBool`, `SetString`/`GetString`, `SerializeToString`/`DeserializeFromString`, and `SaveToFile`/`LoadFromFile`.

## 3. Demo Validation
```bash
make audio-demo
./build/demos/audio_demo --frames 10

make save-demo
./build/demos/save_demo --frames 10
```
