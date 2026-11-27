#include "engine/core/scene.h"

namespace engine::core {

void SceneManager::RegisterScene(const std::string& name,
                                 std::unique_ptr<Scene> scene) {
  scenes_[name] = std::move(scene);
}

bool SceneManager::SwitchTo(const std::string& name) {
  auto it = scenes_.find(name);
  if (it == scenes_.end()) {
    return false;
  }

  if (current_scene_ != nullptr) {
    current_scene_->OnExit();
  }

  current_name_ = name;
  current_scene_ = it->second.get();

  if (current_scene_ != nullptr) {
    current_scene_->OnEnter();
  }
  return true;
}

void SceneManager::Update(Timestep dt) {
  if (current_scene_ != nullptr) {
    current_scene_->OnUpdate(dt);
  }
}

void SceneManager::Render() {
  if (current_scene_ != nullptr) {
    current_scene_->OnRender();
  }
}

}  // namespace engine::core
