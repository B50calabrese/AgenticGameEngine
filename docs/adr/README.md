# Architecture Decision Records (ADRs)

Architecture Decision Records capture important technical and design decisions, their context, and their consequences.

## Index of Records

* [0001 - Record Architecture Decisions](file:///home/antonio/code/AgenticGameEngine/docs/adr/0001-record-architecture-decisions.md)
* [0002 - Submodule-Friendly Modern CMake and Makefile Architecture](file:///home/antonio/code/AgenticGameEngine/docs/adr/0002-submodule-friendly-cmake-and-makefile.md)
* [0003 - Abstract Window Interface and GLFW Backend](file:///home/antonio/code/AgenticGameEngine/docs/adr/0003-abstract-window-interface-and-glfw-backend.md)
* [0004 - 2D Rendering Primitives & Hardware Abstraction Layer](file:///home/antonio/code/AgenticGameEngine/docs/adr/0004-rendering-primitives-and-hardware-abstraction.md)
* [0005 - Linear Algebra & Spatial Transform Primitives](file:///home/antonio/code/AgenticGameEngine/docs/adr/0005-linear-algebra-and-transform-primitives.md)
* [0006 - Polling Input Architecture & Frame Timestep Clock](file:///home/antonio/code/AgenticGameEngine/docs/adr/0006-polling-input-and-frame-timestep.md)

## ADR Workflow

1. When facing a significant technical decision (API design, subsystem boundaries, external dependency, memory architecture), copy `template.md` to a new sequential file: `XXXX-title-in-kebab-case.md`.
2. The `systems` agent or lead architect drafts the Context and Decision.
3. Review with the user / orchestrator.
4. Mark status as `Accepted`, `Rejected`, or `Superseded`.
