---
name: validate-framework
description: Run DefineJoy AI Badge host tests and report Build, Host tests, Device tests, and Unverified separately. Use before delivery, after Runtime or App hook changes, or when the user asks to verify.
---

<p align="right">
  <a href="SKILL.zh_CN.md">简体中文</a> · <strong>English</strong>
</p>

# Validate the framework

## Host tests (no ESP-IDF)

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

If CMake is missing, compile with any C++17 compiler as in `tests/host/README.md`.

Host tests cover event dispatch order, disabled small-loop, and Idle → Sleep → Wake. They do **not** talk to silicon.

## Firmware build (needs ESP-IDF)

```bash
idf.py set-target esp32s3
idf.py -DBADGE_APP=playground build
```

A successful build is not a device test.

## Delivery block

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: remaining board, instrument, or user checks
```

GPIO is still placeholder (`pinmap.hpp` = -1) until PCB freeze. Unless the user ran hardware, Device tests are NOT RUN.
