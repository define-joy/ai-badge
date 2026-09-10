---
name: extend-hardware
description: 为 AI Badge 新增外设：Port + Adapter + 外观，不改 Runtime。PCB、BOM 或用户引入新芯片、总线、能力时使用。
---

<p align="right">
  <strong>简体中文</strong> · <a href="SKILL.md">English</a>
</p>

# 扩展硬件

## 何时使用

BOM 或原理图出现新器件，或要把空实现换成真驱动。

## 规则

- 改引脚只动 `adapters/board/pinmap.hpp`
- 换芯片只动 `adapters/<capability>/`
- 不要把料号或 GPIO 写进 `kit/include/badge/`
- 除非生命周期本身有错，不要改 `core/Runtime`
- Probe 失败是软失败：整机继续，`available()` 为 false

## 步骤

1. 读 `docs/board-facts.md` 与 `adapters/board/bom_map.md`。引脚未知则停下询问。
2. 在 `ports/Interfaces.hpp` 增加接口。
3. 写 Adapter（保留 `null` 以便 host 测试）。
4. 在 `bringup.cpp` 挂上，并更新 `capabilities.hpp` / `bom_map.md`。
5. 在 `Badge.hpp` / `Badge.cpp` 增加外观与 `badge::foo()`。
6. 若器件会产生事件，Adapter 只入队 `badge::Event`，由 Runtime 分发。
7. 用户可见的能力补 `apps/examples/`。
8. 跑 host 测试。硅片未验证前 Device tests 报 NOT RUN。

## 完成标准

Runtime 未改、玩法仍能编译、新 API 没有引脚参数。
