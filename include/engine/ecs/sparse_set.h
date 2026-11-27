#ifndef ENGINE_ECS_SPARSE_SET_H_
#define ENGINE_ECS_SPARSE_SET_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>
#include <vector>

#include "engine/ecs/entity.h"

namespace engine::ecs {

class ISparseSet {
 public:
  virtual ~ISparseSet() = default;
  virtual void Remove(EntityId entity) = 0;
  virtual bool Has(EntityId entity) const = 0;
  virtual std::size_t Size() const = 0;
};

/**
 * @brief Data-oriented contiguous component storage with O(1) lookup, insertion,
 *        and swap-and-pop removal.
 */
template <typename T>
class SparseSet : public ISparseSet {
 public:
  static constexpr std::uint32_t kInvalidDense =
      std::numeric_limits<std::uint32_t>::max();

  template <typename... Args>
  T& Emplace(EntityId entity, Args&&... args) {
    const std::uint32_t index = EntityIndex(entity);
    if (index >= sparse_.size()) {
      sparse_.resize(index + 1, kInvalidDense);
    }
    if (sparse_[index] != kInvalidDense) {
      dense_data_[sparse_[index]] = T{std::forward<Args>(args)...};
      return dense_data_[sparse_[index]];
    }
    const std::uint32_t dense_idx =
        static_cast<std::uint32_t>(dense_entities_.size());
    sparse_[index] = dense_idx;
    dense_entities_.push_back(entity);
    dense_data_.push_back(T{std::forward<Args>(args)...});
    return dense_data_.back();
  }

  void Remove(EntityId entity) override {
    if (!Has(entity)) {
      return;
    }
    const std::uint32_t index = EntityIndex(entity);
    const std::uint32_t dense_idx = sparse_[index];
    const std::uint32_t last_idx =
        static_cast<std::uint32_t>(dense_entities_.size() - 1);

    if (dense_idx != last_idx) {
      const EntityId last_entity = dense_entities_[last_idx];
      dense_entities_[dense_idx] = last_entity;
      dense_data_[dense_idx] = std::move(dense_data_[last_idx]);
      sparse_[EntityIndex(last_entity)] = dense_idx;
    }

    dense_entities_.pop_back();
    dense_data_.pop_back();
    sparse_[index] = kInvalidDense;
  }

  bool Has(EntityId entity) const override {
    const std::uint32_t index = EntityIndex(entity);
    if (index >= sparse_.size()) {
      return false;
    }
    const std::uint32_t dense_idx = sparse_[index];
    return dense_idx != kInvalidDense && dense_entities_[dense_idx] == entity;
  }

  T& Get(EntityId entity) {
    assert(Has(entity));
    return dense_data_[sparse_[EntityIndex(entity)]];
  }

  const T& Get(EntityId entity) const {
    assert(Has(entity));
    return dense_data_[sparse_[EntityIndex(entity)]];
  }

  T* TryGet(EntityId entity) {
    if (!Has(entity)) {
      return nullptr;
    }
    return &dense_data_[sparse_[EntityIndex(entity)]];
  }

  std::size_t Size() const override { return dense_data_.size(); }
  const std::vector<EntityId>& Entities() const { return dense_entities_; }
  std::vector<T>& Data() { return dense_data_; }
  const std::vector<T>& Data() const { return dense_data_; }

 private:
  std::vector<std::uint32_t> sparse_;
  std::vector<EntityId> dense_entities_;
  std::vector<T> dense_data_;
};

}  // namespace engine::ecs

#endif  // ENGINE_ECS_SPARSE_SET_H_
