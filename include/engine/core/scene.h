#ifndef ENGINE_CORE_SCENE_H_
#define ENGINE_CORE_SCENE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "engine/core/timestep.h"

namespace engine::core {

/**
 * @brief Base interface for game scenes / states (Menu, Combat, Pause, etc.).
 */
class Scene {
 public:
  virtual ~Scene() = default;

  virtual void OnEnter() {}
  virtual void OnExit() {}
  virtual void OnUpdate(Timestep dt) { (void)dt; }
  virtual void OnRender() {}
};

/**
 * @brief Manages registered scenes and transitions between them.
 */
class SceneManager {
 public:
  void RegisterScene(const std::string& name, std::unique_ptr<Scene> scene);
  bool SwitchTo(const std::string& name);

  void Update(Timestep dt);
  void Render();

  const std::string& GetCurrentSceneName() const { return current_name_; }
  Scene* GetCurrentScene() const { return current_scene_; }

 private:
  std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
  std::string current_name_;
  Scene* current_scene_ = nullptr;
};

}  // namespace engine::core

#endif  // ENGINE_CORE_SCENE_H_
