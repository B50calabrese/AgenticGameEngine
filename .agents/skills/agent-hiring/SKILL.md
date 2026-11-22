---
name: agent-hiring
description: Standard Operating Procedure (SOP) for evaluating capability gaps, drafting agent specifications, and onboarding new agents or skills with human approval.
---

# Agent Hiring & Governance Skill

This skill guides the **HR Agent** and **Orchestrator** in evaluating whether to onboard a new agent persona or create a skill, and outlines the mandatory procedure for doing so.

---

## 1. Hiring vs. Skill Threshold Decision Tree

Agents in this workspace must remain **strictly bounded and hyper-focused**. Never create an agent when a skill suffices.

```text
Do we have a new operational need or subsystem requirement?
│
├── Is this a procedure, runbook, or domain guideline that an existing agent
│   (orchestrator, hr, systems, coding, optimizer, docs) can execute?
│   │
│   └── YES ──► CREATE A SKILL under `.agents/skills/<skill-name>/SKILL.md`
│
└── Does the task require:
    - A fundamentally distinct persona (e.g. specialized Shader Artist, Audio Designer, Physics Sim Specialist)?
    - A dedicated, isolated system instruction set that would pollute existing agents?
    - Unique tool access restrictions or external system interactions?
    │
    └── YES ──► DRAFT AN AGENT PROPOSAL (Proceed to Step 2)
```

---

## 2. The Agent Proposal Protocol

When a new agent is justified, the HR agent must draft an **Agent Proposal** and present it to the user. **Do not create the file until the user explicitly approves.**

### Proposal Format:
1. **Title & Role Name**: (e.g., `audio-engineer`, `shader-specialist`)
2. **Justification**: Why existing agents (`coding`, `systems`, etc.) cannot handle this without persona degradation.
3. **Primary Mandate**: 2-3 sentence summary of the agent's exact purpose.
4. **Strict Boundaries & Anti-Goals**: What this agent must *never* do (e.g. "Does not modify core windowing or memory allocators").
5. **Tool Grants**: Explicit list of required tools (e.g. `[read_file, edit_file, write_file, run_command]`).
6. **Draft System Prompt**: The complete markdown text to be saved in `.agents/agents/<name>.md`.

---

## 3. Onboarding & Registration Steps (Upon User Approval)

Once the user approves the proposal:

1. **Write Agent Markdown**:
   Create `.agents/agents/<name>.md` with valid YAML frontmatter:
   ```markdown
   ---
   name: <name>
   description: <clear third-person description>
   subagent: true
   mainAgent: false
   tools: [<tools>]
   ---
   # <Role Name> - System Instructions
   ...
   ```
2. **Update Documentation**:
   * Add the new agent to the roster table in [AGENTS.md](file:///home/antonio/code/AgenticGameEngine/AGENTS.md).
   * Record the addition in the current session log in [TASKS.md](file:///home/antonio/code/AgenticGameEngine/TASKS.md).
3. **Validation Smoke Test**:
   * Test dispatching a brief sample query to the new subagent to verify it initializes cleanly and adheres to its persona.
