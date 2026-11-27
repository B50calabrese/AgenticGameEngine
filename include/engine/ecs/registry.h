#ifndef ENGINE_ECS_REGISTRY_H_
#define ENGINE_ECS_REGISTRY_H_

#include <memory>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

#include "engine/ecs/entity.h"
#include "engine/ecs/sparse_set.h"

namespace engine::ecs {

/**
 * @brief Central ECS coordinator managing entities, component pools, Views,
 *        and deferred destruction.
 */
class Registry {
 public:
  EntityId CreateEntity() { return entities_.Create(); }

  void DestroyEntity(EntityId entity) {
    if (!entities_.IsAlive(entity)) {
      return;
    }
    for (auto& [type, pool] : pools_) {
      pool->Remove(entity);
    }
    entities_.Destroy(entity);
  }

  void QueueDestroy(EntityId entity) { destroy_queue_.push_back(entity); }

  void FlushDestroyed() {
    for (EntityId entity : destroy_queue_) {
      DestroyEntity(entity);
    }
    destroy_queue_.clear();
  }

  bool IsAlive(EntityId entity) const { return entities_.IsAlive(entity); }
  std::size_t AliveCount() const { return entities_.AliveCount(); }

  template <typename T, typename... Args>
  T& AddComponent(EntityId entity, Args&&... args) {
    return GetOrCreatePool<T>()->Emplace(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  void RemoveComponent(EntityId entity) {
    if (auto* pool = GetPool<T>()) {
      pool->Remove(entity);
    }
  }

  template <typename T>
  bool HasComponent(EntityId entity) const {
    const auto* pool = GetPool<T>();
    return pool != nullptr && pool->Has(entity);
  }

  template <typename T>
  T& GetComponent(EntityId entity) {
    return GetPool<T>()->Get(entity);
  }

  template <typename T>
  const T& GetComponent(EntityId entity) const {
    return GetPool<T>()->Get(entity);
  }

  template <typename... Ts>
  class View {
   public:
    explicit View(std::tuple<SparseSet<Ts>*...> pools) : pools_(pools) {}

    template <typename Fn>
    void Each(Fn&& fn) {
      auto* primary = std::get<0>(pools_);
      if (primary == nullptr) {
        return;
      }
      const auto& entities = primary->Entities();
      for (std::size_t i = 0; i < entities.size(); ++i) {
        const EntityId entity = entities[i];
        if ((std::get<SparseSet<Ts>*>(pools_)->Has(entity) && ...)) {
          fn(entity, std::get<SparseSet<Ts>*>(pools_)->Get(entity)...);
        }
      }
    }

   private:
    std::tuple<SparseSet<Ts>*...> pools_;
  };

  template <typename... Ts>
  View<Ts...> GetView() {
    return View<Ts...>(std::make_tuple(GetPool<Ts>()...));
  }

 private:
  template <typename T>
  SparseSet<T>* GetOrCreatePool() {
    const std::type_index key(typeid(T));
    auto it = pools_.find(key);
    if (it == pools_.end()) {
      auto pool = std::make_unique<SparseSet<T>>();
      SparseSet<T>* raw = pool.get();
      pools_.emplace(key, std::move(pool));
      return raw;
    }
    return static_cast<SparseSet<T>*>(it->second.get());
  }

  template <typename T>
  SparseSet<T>* GetPool() const {
    const std::type_index key(typeid(T));
    auto it = pools_.find(key);
    if (it == pools_.end()) {
      return nullptr;
    }
    return static_cast<SparseSet<T>*>(it->second.get());
  }

  EntityManager entities_;
  std::unordered_map<std::type_index, std::unique_ptr<ISparseSet>> pools_;
  std::vector<EntityId> destroy_queue_;
};

}  // namespace engine::ecs

#endif  // ENGINE_ECS_REGISTRY_H_
