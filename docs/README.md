# Engine Documentation Wiki

Welcome to the **Agentic Game Engine Wiki**. This wiki serves as the single source of truth for the engine's architecture, subsystem designs, public API contracts, coding standards, and decision records.

> **Agent Mandate**: Any significant change (new subsystem, API evolution, build configuration change, or architectural decision) **must update this wiki** before the task can be marked `DONE`.

---

## 📚 Wiki Directory & Navigation

```text
docs/
├── README.md                      # [You Are Here] Wiki Home & Master Portal
├── architecture/                  # High-level architecture & system design
│   ├── overview.md                # System topology, layers, and data flow
│   └── cross_platform.md          # Platform abstraction principles
├── subsystems/                    # Living documentation for each subsystem
│   ├── core.md                    # Core utilities, logging, version, assertions
│   ├── platform.md                # Window abstraction, events, context lifecycle
│   ├── renderer.md                # Hardware Abstraction Layer & 2D Primitive Batching
│   ├── ecs.md                     # Entity Component System (Upcoming)
│   ├── physics.md                 # 2D Physics & Collision (Upcoming)
│   └── scripting.md               # Lua Integration & Bindings (Upcoming)
├── guides/                        # Practical guides for consumers & developers
│   └── submodule_guide.md         # Embedding the engine in downstream games
├── standards/                     # Quality, style, and maintenance standards
│   ├── cpp_guidelines.md          # Google C++ conventions for engine code
│   └── wiki_maintenance.md        # Living Wiki update protocol & rules
└── adr/                           # Architecture Decision Records
    ├── README.md                  # ADR Index
    ├── 0001-record-architecture-decisions.md
    ├── 0002-submodule-friendly-cmake-and-makefile.md
    ├── 0003-abstract-window-interface-and-glfw-backend.md
    └── 0004-rendering-primitives-and-hardware-abstraction.md
```

---

## 🗺️ Quick Links by Role

### 🎮 Downstream Game Creators (Submodule Consumers)
* [Submodule Consumer Guide](guides/submodule_guide.md): How to embed the engine via `git submodule` and CMake.
* [Subsystems Overview](architecture/overview.md): What capabilities the engine provides.
* [Core Subsystem](subsystems/core.md): Logging, assertions, and version APIs.

### 🛠️ Engine Contributors & Subagents
* [Wiki Maintenance Protocol](standards/wiki_maintenance.md): Rules for keeping docs in lockstep with code.
* [C++ Guidelines & Style](standards/cpp_guidelines.md): Google C++ Style Guide rules.
* [Architecture Decision Records (ADRs)](adr/README.md): Read the history and rationale behind foundational technical choices.
