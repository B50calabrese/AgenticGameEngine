#ifndef ENGINE_ECS_REGISTRY_H_
#define ENGINE_ECS_REGISTRY_H_

#include <algorithm>
#include <functional>
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
      if (pool->Has(entity)) {
        auto sig_it = on_destroy_.find(type);
        if (sig_it != on_destroy_.end()) {
          for (auto& cb : sig_it->second) {
            cb(*this, entity);
          }
        }
        pool->Remove(entity);
      }
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
    T& comp = GetOrCreatePool<T>()->Emplace(entity, std::forward<Args>(args)...);
    auto it = on_construct_.find(std::type_index(typeid(T)));
    if (it != on_construct_.end()) {
      for (auto& cb : it->second) {
        cb(*this, entity);
      }
    }
    return comp;
  }

  template <typename T>
  void RemoveComponent(EntityId entity) {
    if (auto* pool = GetPool<T>()) {
      if (pool->Has(entity)) {
        auto it = on_destroy_.find(std::type_index(typeid(T)));
        if (it != on_destroy_.end()) {
          for (auto& cb : it->second) {
            cb(*this, entity);
          }
        }
        pool->Remove(entity);
      }
    }
  }

  template <typename T>
  T* TryGetComponent(EntityId entity) {
    auto* pool = GetPool<T>();
    return pool != nullptr ? pool->TryGet(entity) : nullptr;
  }

  template <typename T>
  const T* TryGetComponent(EntityId entity) const {
    const auto* pool = GetPool<T>();
    return pool != nullptr ? pool->TryGet(entity) : nullptr;
  }

  using SignalCallback = std::function<void(Registry&, EntityId)>;

  template <typename T>
  void OnConstruct(SignalCallback cb) {
    on_construct_[std::type_index(typeid(T))].push_back(std::move(cb));
  }

  template <typename T>
  void OnDestroy(SignalCallback cb) {
    on_destroy_[std::type_index(typeid(T))].push_back(std::move(cb));
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
      if (((std::get<SparseSet<Ts>*>(pools_) == nullptr) || ...)) {
        return;
      }
      const std::vector<EntityId>* smallest = nullptr;
      std::size_t min_size = std::numeric_limits<std::size_t>::max();
      auto check_pool = [&](const auto* pool) {
        if (pool->Size() < min_size) {
          min_size = pool->Size();
          smallest = &pool->Entities();
        }
      };
      (check_pool(std::get<SparseSet<Ts>*>(pools_)), ...);

      for (std::size_t i = 0; i < smallest->size(); ++i) {
        const EntityId entity = (*smallest)[i];
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
  std::unordered_map<std::type_index, std::vector<SignalCallback>> on_construct_;
  std::unordered_map<std::type_index, std::vector<SignalCallback>> on_destroy_;
  std::vector<EntityId> destroy_queue_;
};

}  // namespace engine::ecs

#endif  // ENGINE_ECS_REGISTRY_H_
