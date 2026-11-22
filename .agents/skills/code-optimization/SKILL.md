---
name: code-optimization
description: Performance engineering guidelines, cache-locality, data-oriented design (DOD), and profiling practices for cross-hardware performance.
---

# Code Optimization & Performance Engineering Skill

This skill guides the **Optimizer Agent** and **Coding Agent** in writing performant, cache-friendly, and latency-sensitive game engine code.

---

## 1. Data-Oriented Design (DOD) & Memory Hierarchy

1. **Contiguous Memory Storage**:
   * Store entities and components in contiguous arrays (`std::vector` / flat buffers) rather than linked lists or node trees.
   * Maximize L1/L2/L3 CPU cache hit rates during frame update loops.
2. **Avoid Pointer-Chasing**:
   * Systems must iterate over compact component arrays directly rather than traversing heap-allocated entity pointers.
3. **Component Layout**:
   * Keep components Plain Old Data (POD) structs.
   * Group frequently accessed fields together and align structures to cache-line boundaries (64 bytes) where beneficial.

---

## 2. Zero-Allocation Inner Loops

1. **Frame Loop Budget**:
   * **Rule**: Zero heap allocations (`malloc`, `new`, `std::vector::push_back` beyond capacity) during the frame tick (`Update()`, `Render()`, physics stepping).
   * Pre-allocate buffers, dynamic arrays, and render queues during initialization or scene loading.
2. **Temporary Containers**:
   * Use reusable scratch buffers or static capacity ring-buffers for per-frame temporary calculations (e.g., collision pairs, frustum-culled instance IDs).

---

## 3. Renderer & OpenGL Batching Optimization

1. **Minimize Draw Calls**:
   * Never issue individual `glDrawArrays` / `glDrawElements` calls per sprite or mesh instance.
   * Route all draw commands through a `RenderQueue` that sorts by:
     1. Shader Program
     2. Texture Binding
     3. Z-depth (front-to-back for opaque to maximize early-Z rejection; back-to-front for transparent)
2. **State Caching**:
   * Cache OpenGL state (active texture unit, bound program, blend mode, depth test status) to prevent redundant driver state changes.
3. **Vertex Buffer Streaming**:
   * Use batched dynamic VBOs (e.g. 10,000 quads per batch) mapped with `glBufferSubData` or persistently mapped buffers.

---

## 4. Multi-Machine & Low-Spec Awareness

* **Baseline**: The engine must maintain solid 60 FPS on modest integrated GPUs (e.g., Intel Iris/UHD, AMD Radeon Vega).
* **Algorithmic Scaling**:
  * Collision detection must use spatial partitioning (e.g. uniform grid, BVH) to maintain $O(n \log n)$ or $O(n)$ scaling instead of naive $O(n^2)$.
* **Benchmarking & Profiling**:
  * Include high-resolution timers (`std::chrono::steady_clock`) around core subsystems (ECS Tick, Physics Tick, Render Submission, GPU Swap).
  * Always benchmark release builds (`-O3 -DNDEBUG`) when verifying optimization gains.
