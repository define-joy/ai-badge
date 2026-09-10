---
name: validate-framework
description: 运行 DefineJoy AI Badge 的 host 测试，并分开报告 Build、Host tests、Device tests、Unverified。交付前、改 Runtime 或钩子后、用户要求验收时使用。
---

<p align="right">
  <strong>简体中文</strong> · <a href="SKILL.md">English</a>
</p>

# 验收框架

## Host 测试（不需要 ESP-IDF）

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

没有 CMake 时按 `tests/host/README.md` 用任意 C++17 编译器。

Host 测试覆盖事件分发顺序、关闭小循环、Idle → Sleep → Wake。它们**不**访问硅片。

## 固件构建（需要 ESP-IDF）

```bash
idf.py set-target esp32s3
idf.py -DBADGE_APP=playground build
```

编译通过不是真机验收。

## 交付格式

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: 仍需板卡、仪器或用户确认的事项
```

引脚仍是占位（`pinmap.hpp` 为 -1）。用户未上板时 Device tests 报 NOT RUN。
