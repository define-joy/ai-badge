---
name: issue-suggestions
description: After a DefineJoy AI Badge release, collect the developer's own improvement points and file GitHub issues on define-joy/ai-badge. Use only when the user explicitly asks to collect suggestions. Confirm consent before drafting or submitting.
---

<p align="right">
  <a href="SKILL.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Collect suggestions and file issues

Turns the releasing developer's own improvement points into issues on `define-joy/ai-badge`. Run only when the developer explicitly asks.

## Safety gate (first)

1. Confirm consent. If they decline, stop.
2. Check GitHub access: `gh auth status`, then `gh`. If none works, give paste-ready issue text and stop.
3. Never submit before review. Draft, show, wait for approval.
4. No credentials, device secrets, personal data, or unsanitized logs.

## Collect

Pain points in the kit API, Runtime, adapters, build, or docs. Do not scrape the public issue tracker as the source. Deduplicate. Search existing issues on `define-joy/ai-badge` before creating.

## Submit

After approval: `gh issue create --repo define-joy/ai-badge` (or the first working GitHub channel). Read the created issue back.

This skill does not publish firmware, open code PRs, or auto-submit.
