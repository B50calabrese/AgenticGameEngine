# Wiki Maintenance & Documentation Invariant Protocol

This document codifies the rules and standard operating procedures for maintaining the engine wiki as a living, authoritative knowledge base.

---

## 1. The Documentation Invariant (Definition of Done)

> **Mandate**: No significant architectural, subsystem, build system, or API change can be marked `DONE` without synchronizing the documentation wiki in `docs/`.

### What Constitutes a "Significant Change"?
A task is considered a significant change if it meets any of the following criteria:
1. **New Subsystem or Feature**: Adds a new module (e.g., Math library, ECS, Renderer, Audio).
2. **Public API Evolution**: Adds, modifies, or deprecates classes/functions in `include/engine/`.
3. **Architectural Divergence**: Shifts memory models, alters frame lifecycle, or changes subsystem interactions.
4. **Build & Dependency Changes**: Adds a third-party dependency, changes CMake targets, or modifies Makefile options.

---

## 2. Wiki Synchronization Checklist

Whenever an agent or developer delivers a significant change, they must execute this checklist before closing the task:

- [ ] **Subsystem Page (`docs/subsystems/<name>.md`)**:
  - Purpose and scope clearly documented.
  - Public API functions and usage examples provided.
  - Invariants, gotchas, and thread-safety constraints documented.
- [ ] **Architecture Map (`docs/architecture/overview.md`)**:
  - Updated if subsystem layering, communication patterns, or dependencies changed.
- [ ] **Master Index (`docs/README.md`)**:
  - Link to the new or updated page added to the central navigation index.
- [ ] **Consumer Impact (`docs/guides/submodule_guide.md` or `README.md`)**:
  - Updated if the change affects how downstream games embed or initialize the engine.
- [ ] **Architecture Decision Record (`docs/adr/`)**:
  - If the change involved choosing between multiple architectural alternatives, author an ADR using `docs/adr/template.md`.

---

## 3. Formatting & Linking Standards

1. **Relative Markdown Links**:
   All internal wiki links must use standard relative markdown links (e.g. `[Core Subsystem](../subsystems/core.md)` or `[Overview](overview.md)`).
2. **Code Snippets**:
   Every wiki page documenting an API must include at least one realistic, compilable C++ code snippet showing proper usage.
3. **Cross-Referencing**:
   Always link related subsystems, ADRs, and guidelines to ensure the wiki remains a hyperlinked web of knowledge rather than isolated silos.
