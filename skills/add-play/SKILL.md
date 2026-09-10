---
name: add-play
description: Create a new DefineJoy AI Badge play under apps/ with BADGE_APP. Use when the user wants a new app, example, game, or firmware experience without touching pins or drivers.
---

<p align="right">
  <a href="SKILL.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Add a play

## When to use

The user wants a new firmware experience (game, demo, tool) on AI Badge.

## Steps

1. Read `docs/framework.md` and `kit/include/badge/App.hpp`.
2. Create `apps/<name>/app.cpp` (or `apps/examples/<name>/app.cpp`).
3. Subclass `badge::App`. Implement only the hooks you need. Default `loopPeriodMs()` is `0`.
4. Register with `BADGE_APP(YourClass);` — do not edit a central demo table.
5. Call hardware only through `badge::screen()`, `haptic()`, `leds()`, and other Facade methods. Never include `pinmap.hpp`.
6. Keep hooks non-blocking. Use `badge::jobs().post(...)` for TF, audio, NFC.
7. Build with `idf.py -DBADGE_APP=<name> build` when ESP-IDF is available. Host tests do not replace device checks.

## Done when

- One `app.cpp` plus `BADGE_APP`
- No GPIO, no `app_main`, no Runtime edits
