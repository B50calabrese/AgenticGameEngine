#include <gtest/gtest.h>

#include "engine/ecs/registry.h"

namespace engine::ecs {
namespace {

struct Position {
  float x = 0.0f;
  float y = 0.0f;
};

struct Velocity {
  float vx = 0.0f;
  float vy = 0.0f;
};

TEST(EntityManagerTest, GenerationalRecyclingInvalidatesOldIds) {
  EntityManager mgr;
  EntityId e1 = mgr.Create();
  EXPECT_TRUE(mgr.IsAlive(e1));
  EXPECT_EQ(mgr.AliveCount(), 1u);

  mgr.Destroy(e1);
  EXPECT_FALSE(mgr.IsAlive(e1));
  EXPECT_EQ(mgr.AliveCount(), 0u);

  EntityId e2 = mgr.Create();
  EXPECT_TRUE(mgr.IsAlive(e2));
  EXPECT_FALSE(mgr.IsAlive(e1));
  EXPECT_EQ(EntityIndex(e1), EntityIndex(e2));
  EXPECT_NE(EntityGeneration(e1), EntityGeneration(e2));
}

TEST(SparseSetTest, SwapAndPopPreservesContiguousData) {
  SparseSet<int> set;
  EntityId e0 = MakeEntityId(0, 0);
  EntityId e1 = MakeEntityId(1, 0);
  EntityId e2 = MakeEntityId(2, 0);

  set.Emplace(e0, 10);
  set.Emplace(e1, 20);
  set.Emplace(e2, 30);
  EXPECT_EQ(set.Size(), 3u);

  set.Remove(e1);
  EXPECT_EQ(set.Size(), 2u);
  EXPECT_FALSE(set.Has(e1));
  EXPECT_EQ(set.Get(e0), 10);
  EXPECT_EQ(set.Get(e2), 30);
}

TEST(RegistryTest, ViewAndDeferredDestructionWorkCorrectly) {
  Registry reg;
  EntityId a = reg.CreateEntity();
  EntityId b = reg.CreateEntity();

  reg.AddComponent<Position>(a, 1.0f, 2.0f);
  reg.AddComponent<Velocity>(a, 3.0f, 4.0f);
  reg.AddComponent<Position>(b, 10.0f, 20.0f);

  int visited = 0;
  reg.GetView<Position, Velocity>().Each([&](EntityId, Position& p, Velocity& v) {
    p.x += v.vx;
    p.y += v.vy;
    ++visited;
  });

  EXPECT_EQ(visited, 1);
  EXPECT_FLOAT_EQ(reg.GetComponent<Position>(a).x, 4.0f);
  EXPECT_FLOAT_EQ(reg.GetComponent<Position>(b).x, 10.0f);

  reg.QueueDestroy(a);
  EXPECT_TRUE(reg.IsAlive(a));
  reg.FlushDestroyed();
  EXPECT_FALSE(reg.IsAlive(a));
  EXPECT_EQ(reg.AliveCount(), 1u);
}

}  // namespace
}  // namespace engine::ecs
