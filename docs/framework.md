# DefineJoy AI Badge 用户开发底座

面向玩法作者：只写 `badge::App` 钩子，通过 `badge::` 外观调用硬件。不要 include `adapters/` 或 `ports/`。

协议为 MIT，Copyright (c) 2026 DefineJoy。

## 最小应用

把文件放在 `apps/playground/app.cpp`（或新建 `apps/<名字>/app.cpp`）：

```cpp
#include <badge/App.hpp>
#include <badge/Badge.hpp>

class HelloApp : public badge::App {
  void onStart() override {
    badge::screen().showText("Hello");
    badge::haptic().pulse(80);
  }
  void onButton(badge::Button btn, badge::ButtonEvent ev) override {
    if (ev == badge::ButtonEvent::Click) badge::leds().next();
  }
};

BADGE_APP(HelloApp);
```

编译当前玩法：

```text
idf.py set-target esp32s3
idf.py -DBADGE_APP=playground build
idf.py -DBADGE_APP=examples/buttons build
```

## 生命周期（模板方法）

框架固定大循环，用户只填钩子。

1. 上电 / 唤醒原因
2. 总线 bringup（只建一次）
3. Probe 各外设：失败不中止，对应 `available()==false`
4. 驱动回调只允许入队，禁止直接调用户虚函数
5. `onStart()`
6. 事件泵：有事件则分发钩子；否则若 `loopPeriodMs()>0` 则 `onLoop()`
7. 电源策略：无小循环且无事件 → `onIdle` → `onSleep`；事件唤醒 → `onWake`
8. 退出：`onStop()`，预留下电

### 小循环

- `loopPeriodMs()` 默认 `0`：纯事件驱动，允许自动休眠。
- 返回 `N>0`：约每 N 毫秒调用 `onLoop()`，此时不自动 Sleep。
- `onLoop` 必须立刻返回。阻塞读写用 `badge::jobs().post(...)`。

## 外观 API

| 方法 | 用途 |
| --- | --- |
| `screen()` | 文本 / 亮度 / 清屏；`runUi` 预留给 LVGL（锁由框架持有） |
| `buttons()` | 轮询是否在线；主路径仍是 `onButton` |
| `haptic()` | `pulse` / `pattern` |
| `imu()` / `onImu` | MPU-6050 手势与姿态 |
| `nfc()` / `onNfc` | ST25DV 动态标签，不是被动 NTAG |
| `leds()` | WS2812 |
| `storage()` | TF 卡虚拟路径 |
| `audio()` | 麦 + 功放 |
| `battery()` | 充电状态；本板无电量计，SOC 可能为 -1 |
| `radio()` | Wi-Fi / BLE 封装（骨架阶段仅 available） |
| `jobs()` | 投递阻塞任务 |
| `power()` | `stayAwake` / `requestSleep` |

所有外观：**无 GPIO 参数**；失败返回 `badge::Status`，不抛异常。

## 扩展（PCB / 新器件）

- 改引脚：只改 `adapters/board/pinmap.hpp`
- 换芯片：只换对应 Adapter 实现，Port 与 App 不动
- 新能力：新 Port + Adapter + `Badge` 上一个方法。不要改 Runtime

## 不要做的事

- 不要 `#include` pinmap、ESP-IDF 驱动头、LVGL 锁
- 不要在钩子里阻塞（录音、写卡、NFC 轮询）
- 不要把编译通过当成真机验收

板级事实优先级见 [board-facts.md](board-facts.md)。
