#ifndef ENGINE_ECS_ENTITY_H_
#define ENGINE_ECS_ENTITY_H_

#include <cstdint>
#include <limits>
#include <vector>

namespace engine::ecs {

using EntityId = std::uint32_t;

constexpr EntityId kNullEntity = std::numeric_limits<EntityId>::max();
constexpr std::uint32_t kEntityIndexBits = 20;
constexpr std::uint32_t kEntityIndexMask = (1u << kEntityIndexBits) - 1u;
constexpr std::uint32_t kEntityGenMask = 0xFFFu;

inline std::uint32_t EntityIndex(EntityId id) {
  return id & kEntityIndexMask;
}

inline std::uint32_t EntityGeneration(EntityId id) {
  return (id >> kEntityIndexBits) & kEntityGenMask;
}

inline EntityId MakeEntityId(std::uint32_t index, std::uint32_t generation) {
  return (index & kEntityIndexMask) |
         ((generation & kEntityGenMask) << kEntityIndexBits);
}

/**
 * @brief Manages entity ID allocation, generational versioning, and recycling.
 */
class EntityManager {
 public:
  EntityId Create() {
    std::uint32_t index = 0;
    if (!free_indices_.empty()) {
      index = free_indices_.back();
      free_indices_.pop_back();
    } else {
      index = static_cast<std::uint32_t>(generations_.size());
      generations_.push_back(0);
    }
    ++alive_count_;
    return MakeEntityId(index, generations_[index]);
  }

  void Destroy(EntityId id) {
    if (!IsAlive(id)) {
      return;
    }
    const std::uint32_t index = EntityIndex(id);
    generations_[index] = (generations_[index] + 1) & kEntityGenMask;
    free_indices_.push_back(index);
    --alive_count_;
  }

  bool IsAlive(EntityId id) const {
    if (id == kNullEntity) {
      return false;
    }
    const std::uint32_t index = EntityIndex(id);
    if (index >= generations_.size()) {
      return false;
    }
    return generations_[index] == EntityGeneration(id);
  }

  std::size_t AliveCount() const { return alive_count_; }

 private:
  std::vector<std::uint32_t> generations_;
  std::vector<std::uint32_t> free_indices_;
  std::size_t alive_count_ = 0;
};

}  // namespace engine::ecs

#endif  // ENGINE_ECS_ENTITY_H_
