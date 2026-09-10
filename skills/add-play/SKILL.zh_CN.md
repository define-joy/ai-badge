---
name: add-play
description: 在 apps/ 下为 DefineJoy AI Badge 新建玩法并用 BADGE_APP 注册。用户要新应用、示例、游戏或固件体验且不应改引脚时使用。
---

<p align="right">
  <strong>简体中文</strong> · <a href="SKILL.md">English</a>
</p>

# 新增玩法

## 何时使用

用户要在 AI Badge 上增加新的固件体验（游戏、示例、工具）。

## 步骤

1. 阅读 `docs/framework.md` 与 `kit/include/badge/App.hpp`。
2. 创建 `apps/<name>/app.cpp`（或 `apps/examples/<name>/app.cpp`）。
3. 继承 `badge::App`，只实现需要的钩子。`loopPeriodMs()` 默认 `0`。
4. 用 `BADGE_APP(YourClass);` 注册，不要改中央 demo 表。
5. 硬件只通过 `badge::screen()`、`haptic()`、`leds()` 等外观调用。禁止 include `pinmap.hpp`。
6. 钩子禁止阻塞。TF / 音频 / NFC 走 `badge::jobs().post(...)`。
7. 有 ESP-IDF 时用 `idf.py -DBADGE_APP=<name> build`。host 测试不能代替真机。

## 完成标准

- 一个 `app.cpp` 加 `BADGE_APP`
- 无 GPIO、无 `app_main`、无 Runtime 改动
