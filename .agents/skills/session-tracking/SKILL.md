---
name: session-tracking
description: Protocol for restoring context at session start, updating active tasks, and recording clean handoffs at session end.
---

# Cross-Session Task Tracking Skill

This skill ensures seamless operational continuity across independent chat sessions and agent invocations.

---

## 1. Session Initialization Protocol (Start of Session)

When starting a new session or turn in this workspace, execute this 3-step context restoration:

1. **Read [AGENTS.md](file:///home/antonio/code/AgenticGameEngine/AGENTS.md)**: Refresh project rules, Google C++ guidelines, and permitted operations.
2. **Read [TASKS.md](file:///home/antonio/code/AgenticGameEngine/TASKS.md)**:
   * Review the **Session Handoff Log** at the bottom of `TASKS.md` to see what the previous session accomplished and what immediate next step was identified.
   * Inspect the **Active Task Board** to identify `IN_PROGRESS` and `TODO` items.
3. **Verify Git & Build State**:
   * Check git status (`git status -s`) to confirm whether the working tree is clean or has uncommitted work.
   * If in the middle of a build milestone, verify build health (`make test` or `ctest`).

---

## 2. In-Session State Updates

Whenever a task changes state:
* Transition status from `TODO` $\rightarrow$ `IN_PROGRESS` $\rightarrow$ `REVIEW` $\rightarrow$ `DONE` (or `BLOCKED`).
* If blocked, clearly state the blocking dependency in the table.
* Never leave a task in `DONE` without compiling code and passing tests.

---

## 3. Session Handoff Protocol (End of Session)

Before concluding a session or completing a major milestone, append a new log entry to the **Session Handoff Log** in [TASKS.md](file:///home/antonio/code/AgenticGameEngine/TASKS.md):

```markdown
### Session: YYYY-MM-DD (Session N - Brief Title)
* **Goal**: [What this session set out to do]
* **Completed**:
  * [Item 1 with file links]
  * [Item 2 with file links]
* **In Progress / Current Blockers**:
  * [Task ID]: [Status or blocker explanation]
* **Immediate Next Step for Next Session**:
  * [Exact first action the next agent/session should take]
```

This guarantees that any agent resuming work in a future turn can hit the ground running with zero wasted turns or confusion.
