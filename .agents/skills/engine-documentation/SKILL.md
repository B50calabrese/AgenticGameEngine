---
name: engine-documentation
description: Standards and procedures for maintaining the living documentation wiki, ADRs, Doxygen comments, and consumer guides.
---

# Engine Documentation & Living Wiki Protocol Skill

This skill guides the **Documentation Agent**, **Orchestrator**, and contributors in authoring, organizing, and maintaining the engine's documentation as a living wiki.

---

## 1. The Living Wiki Structure (`docs/`)

The `docs/` directory is organized as an interconnected, navigable knowledge base:

```text
docs/
├── README.md                  # Master Wiki Portal & Category Index
├── architecture/              # High-level system topology and layer interactions
│   └── overview.md
├── subsystems/                # Deep-dive pages for every engine subsystem
│   ├── core.md
│   ├── ecs.md (upcoming)
│   ├── renderer.md (upcoming)
│   └── ...
├── guides/                    # Practical guides for consumers and contributors
│   └── submodule_guide.md
├── standards/                 # Quality, style, and maintenance standards
│   ├── cpp_guidelines.md
│   └── wiki_maintenance.md
└── adr/                       # Architectural Decision Records
    ├── README.md
    └── XXXX-title.md
```

---

## 2. Standard Subsystem Wiki Template

Every subsystem document in `docs/subsystems/<name>.md` must follow this structure:

```markdown
# [Subsystem Name] Subsystem

[One-sentence overview of the subsystem's responsibility]

* **Status**: [Proposed | In Progress | Active & Implemented]
* **Public Headers**: `include/engine/<subsystem>/`
* **Internal Source**: `src/engine/<subsystem>/`
* **Tests**: `tests/<subsystem>_test.cpp`

---

## 1. Architectural Role & Responsibilities
[Why this subsystem exists and what it owns]

## 2. Public API & Key Classes
[Descriptions of primary classes and methods]

## 3. Usage Example
[Compilable, clean C++ code snippet demonstrating typical usage]

## 4. Invariants, Threading & Memory Ownership
[Rules, allocation patterns, and lifecycle constraints]

## 5. Related ADRs
- [Link to relevant ADRs]
```

---

## 3. Definition of Done: Wiki Synchronization

Before any epic, milestone, or major feature is marked `DONE`:
1. **Subsystem Page**: Created or updated in `docs/subsystems/`.
2. **Master Index**: Checked in `docs/README.md` to ensure the link is present.
3. **Consumer Impact**: If public API changed, update `docs/guides/submodule_guide.md`.
4. **ADR Written**: If a foundational decision was made, document in `docs/adr/`.
