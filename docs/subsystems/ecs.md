# Entity Component System (ECS)

The **ECS** subsystem (`engine::ecs`) provides a data-oriented, cache-friendly Entity Component System built on contiguous `SparseSet<T>` pools and generational `EntityId`s.

---

## 1. Public API Surface

Public headers live in `include/engine/ecs/`:

| Header | Class / Type | Description |
| :--- | :--- | :--- |
| `entity.h` | `EntityId`, `EntityManager` | 32-bit generational IDs (20-bit index, 12-bit generation) with free-list recycling. |
| `sparse_set.h` | `SparseSet<T>`, `ISparseSet` | Contiguous component storage with $O(1)$ lookup, insertion, and swap-and-pop removal. |
| `registry.h` | `Registry`, `Registry::View<...>` | Central coordinator for entity/component lifecycle, `GetView<Ts...>().Each(...)`, and deferred destruction (`QueueDestroy` / `FlushDestroyed`). |

---

## 2. Usage Example

```cpp
engine::ecs::Registry registry;
auto player = registry.CreateEntity();
registry.AddComponent<Transform2D>(player, 100.0f, 200.0f, 32.0f);
registry.AddComponent<Velocity2D>(player, 50.0f, 0.0f);

registry.GetView<Transform2D, Velocity2D>().Each(
    [&](engine::ecs::EntityId, Transform2D& t, Velocity2D& v) {
      t.x += v.vx * dt;
      t.y += v.vy * dt;
    });
```

---

## 3. Demo Validation

```bash
make ecs-demo
./build/demos/ecs_demo --frames 10
```
