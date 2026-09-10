---
name: experience-pr
description: After a DefineJoy AI Badge release, capture reusable learnings as a documentation pull request on define-joy/ai-badge. Use only when the user explicitly asks to collect experience. Confirm consent; never commit on the current branch without a dedicated branch.
---

<p align="right">
  <a href="SKILL.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Collect experience and open a docs PR

Captures durable learnings (hardware facts, API contracts, build pitfalls) into `docs/` on a dedicated branch against `define-joy/ai-badge`.

## Safety gate (first)

1. Confirm consent. If they decline, stop.
2. Check GitHub access (`gh auth status`).
3. Do not commit on the developer's current working branch. Use a new branch (for example `docs/experience-<topic>`).
4. Show the diff and wait for explicit approval before commit, push, or `gh pr create`.
5. No credentials or private data.

## Collect

Keep facts that help the next Badge firmware: pin freeze notes, Adapter behavior, Runtime invariants, test gaps. Drop one-off debug notes.

Prefer updating `docs/board-facts.md`, `docs/framework.md`, or `adapters/board/bom_map.md` over adding a parallel rule file. Keep `AGENTS.md` in sync if routing changes.

## Submit

After approval, Conventional Commit (English imperative), push to `origin`, open a PR to `define-joy/ai-badge` base `main`. Report Build / Host tests / Device tests / Unverified (docs-only → NOT RUN with reason).
