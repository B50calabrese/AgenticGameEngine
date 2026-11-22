---
name: task-breakdown
description: Protocol for decomposing complex engine features and milestones into sized, dependency-mapped, interface-first work packages.
---

# Task Breakdown Skill

This skill guides agents (particularly the **Orchestrator** and **Systems Agent**) in breaking large milestones and complex engine features into smaller, verifiable, and parallelizable tasks.

---

## 1. Principles of Task Decomposition

1. **Interface-First (Contract Before Code)**:
   Never write implementation code before the interface header (`include/engine/...`) and data structures are defined and reviewed.
2. **Atomic & Independent**:
   Tasks should be scoped so that an individual subagent can implement, test, and verify them in a single focused cycle.
3. **Explicit Verification Criteria**:
   Every task must state its definition of done (e.g., "Compiles cleanly under `-Wall -Wextra`, passes unit tests in `tests/ecs_test.cpp`").
4. **Sizing Guardrails**:
   * If a task touches more than 3-4 files or crosses subsystem boundaries, it is too large and must be decomposed further.

---

## 2. Standard 4-Phase Decomposition Cycle

For any major engine feature (e.g., "Add Batch Sprite Renderer" or "Implement Spatial Grid"):

```text
[Phase 1: Architecture & Interface]
  │   - Systems Agent defines public header contracts in `include/engine/`
  │   - Systems/Docs agent records ADR if a novel pattern is introduced
  ▼
[Phase 2: Test Specification]
  │   - Coding Agent writes failing unit tests / mocks in `tests/`
  ▼
[Phase 3: Core Implementation]
  │   - Coding Agent writes implementation in `src/engine/`
  │   - Coding Agent formats with clang-format
  ▼
[Phase 4: Optimization & Verification]
      - Optimizer Agent inspects cache efficiency and loop overhead
      - Orchestrator runs build and test suite, marking task DONE in TASKS.md
```

---

## 3. Formatting Tasks for `TASKS.md`

When adding tasks to the board, format them with consistent IDs and explicit dependencies:

| Task ID | Task Description | Owner | Status | Dependencies |
| :--- | :--- | :--- | :--- | :--- |
| `SUBSYS-01` | Define `ComponentStorage<T>` header and memory layout | Systems | `TODO` | None |
| `SUBSYS-02` | Implement `ComponentStorage<T>` unit tests in Google Test | Coding | `TODO` | `SUBSYS-01` |
| `SUBSYS-03` | Implement `ComponentStorage<T>` contiguous array storage | Coding | `TODO` | `SUBSYS-02` |
| `SUBSYS-04` | Benchmark storage reallocation and cache hit rate | Optimizer | `TODO` | `SUBSYS-03` |
