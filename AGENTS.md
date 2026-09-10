<p align="right">
  <a href="AGENTS.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Repository Guidelines for AI Agents

This file is the only mandatory entry point for AI-assisted work in this repository. Read task-specific documents from the routing table below; do not load every README by default.

## Project and safety baseline

- Brand: **DefineJoy**. Product firmware: **AI Badge**.
- Target: ESP32-S3-WROOM-1-N16R8, 16 MB Flash, 8 MB octal PSRAM. ESP-IDF 5.5.x when building on device.
- User-facing code lives in `apps/`. Stable APIs live in `kit/include/badge/` only. Pins, buses, and chip drivers live in `adapters/` and must not leak into user apps.
- Hardware facts follow this priority: product specifications, BOM, and measured results → `adapters/board/pinmap.hpp` → Adapter implementations → `adapters/board/bom_map.md` → `docs/framework.md`. If a GPIO, address, or register is not defined, ask the user instead of guessing a generic ESP32-S3 board.
- `BADGE_ADAPTER_NULL` is 1 until PCB bring-up. Null adapters may return `Ok` without talking to silicon. Never present a host-test or firmware **build** as device validation.
- Preserve existing user changes. Start with `git status --short --branch`. Do not overwrite unrelated files.
- Hooks (`onButton`, `onLoop`, …) must stay non-blocking. Audio, TF, NFC, and other slow work go through `badge::jobs().post(...)`.
- Do not include `adapters/board/pinmap.hpp`, ESP-IDF driver headers, or LVGL locks from `apps/`.
- Never commit credentials, device secrets, private keys, personal data, or unsanitized logs.
- License is MIT, Copyright (c) 2026 DefineJoy. Keep `LICENSE` when copying substantial portions.

## Task-specific context routing

| Task | Read before editing |
| --- | --- |
| Any code change | `docs/framework.md`, neighboring headers |
| Pins, buses, chips, BOM | `docs/board-facts.md`, `adapters/board/pinmap.hpp`, `adapters/board/bom_map.md` |
| User play / example app | `apps/README.md`, `kit/include/badge/App.hpp`, nearest `apps/**/app.cpp` |
| Runtime / power / events | `core/Runtime.hpp`, `docs/framework.md` |
| New peripheral | skill `extend-hardware` |
| New play | skill `add-play` |
| Host tests | `tests/host/README.md`, skill `validate-framework` |
| Commit or PR | conventional English imperative title; do not push unless asked |

## Required validation and delivery

Iterate with the smallest relevant check. Before delivery, run host tests when C++ logic changed:

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

Device firmware (`idf.py set-target esp32s3` then `idf.py -DBADGE_APP=playground build`) is a separate gate and needs ESP-IDF. Report fields separately:

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: remaining board, instrument, or user checks
```

Create commits and push only when the user requests them.
