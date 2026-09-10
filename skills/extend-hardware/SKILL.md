---
name: extend-hardware
description: Add a new AI Badge peripheral as Port + Adapter + Facade without changing Runtime. Use when PCB, BOM, or the user introduces a new chip, bus, or capability.
---

<p align="right">
  <a href="SKILL.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Extend hardware

## When to use

A new device appears in the BOM or schematic, or an existing stub must become a real driver.

## Rules

- Change pins only in `adapters/board/pinmap.hpp`.
- Change chips only in `adapters/<capability>/`.
- Do not put part numbers or GPIO in `kit/include/badge/`.
- Do not modify `core/Runtime` unless the lifecycle itself is wrong.
- Probe failures are soft: the rest of the device keeps running; `available()` is false.

## Steps

1. Read `docs/board-facts.md` and `adapters/board/bom_map.md`. If the pin is unknown, stop and ask.
2. Add or extend the interface in `ports/Interfaces.hpp`.
3. Implement an Adapter (keep `null` working for host tests).
4. Wire it in `adapters/board/bringup.cpp` and update `capabilities.hpp` / `bom_map.md`.
5. Add a Facade type and `badge::foo()` in `kit/include/badge/Badge.hpp` and `kit/src/Badge.cpp`.
6. If the device raises events, enqueue `badge::Event` from the Adapter; dispatch already exists in Runtime.
7. Add or extend an example under `apps/examples/` if the capability is user-visible.
8. Run host tests. Report Device tests as NOT RUN until silicon is verified.

## Done when

Runtime is unchanged, user apps still compile, and the new API has no pin parameters.
