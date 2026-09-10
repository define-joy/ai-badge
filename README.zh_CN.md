# DefineJoy AI Badge

[English](README.md) | [简体中文](README.zh_CN.md)

DefineJoy AI Badge 是开放式可穿戴 AI 硬件。本仓库是给玩法作者的**开发底座**：硬件事实、稳定外观 API、固定设备生命周期、示例和验收方法放在同一处。应用只写 `badge::App` 钩子，不必写 GPIO、`app_main` 或驱动初始化。

**目前可提供硬件样机。** 请到官网申请：[https://www.definejoy.cn/](https://www.definejoy.cn/)

仓库组织原则：

- `main` 是最小但完整的基线，也是当前硬件能力契约的可执行说明；
- 公开 API 只在 `kit/include/badge/`，引脚和芯片只在 `adapters/`；
- 玩法放在 `apps/`，用 `BADGE_APP` 注册，不要维护中央 demo 表；
- AI 助手入口是 [`AGENTS.zh_CN.md`](AGENTS.zh_CN.md)；板级事实见 [`docs/board-facts.md`](docs/board-facts.md)；用户指南见 [`docs/framework.md`](docs/framework.md)；
- 构建结果与真机结果分开记录，禁止把「编译通过」说成「硬件验证通过」。

PCB 尚未定稿。`adapters/board/pinmap.hpp` 里的 GPIO 仍是占位（`-1`）。当前 Adapter 可以是空实现；`available()` 和 host 测试都不能证明硅片在线。

## 设计理念

底座把 **模板方法**（框架写死生命周期，玩法只填钩子）和 **外观**（硬件藏在 `badge::*` 后面）叠在一起。嵌入式里更现代的叫法是 **端口与适配器（六边形）**：玩法依赖接口，不依赖 GPIO 或 NimBLE。Arduino 的 `setup()` / `loop()` 是同一类抽象；这里骨架还管事件泵和电源策略。

```text
玩法 apps/
  钩子：onStart / onLoop / onButton / onBleMessage / ...
  调用：badge::screen() haptic() radio().ble() ...
        ↓  外观 — 无引脚、无驱动类型
Ports（ports/）     不含 GPIO 的契约
        ↓
Adapters           芯片、pinmap、ESP-IDF、NimBLE
        ↑
Runtime（core/）   玩法不可覆盖的大循环
```

### 模板方法 — 大循环与小循环

`core::Runtime` 是算法骨架。玩法不写 `app_main`，也不自己排初始化顺序。

1. 总线只建一次（幂等）。
2. Probe 各外设。一路失败不中止整机（`available()==false`）。
3. 驱动回调 **只入队**。禁止从 ISR 或 BLE 任务直接调用户虚函数。
4. `onStart()`。
5. 事件泵：分发钩子；无事件且 `loopPeriodMs()>0` 时调 `onLoop()`。
6. 电源：无小循环且无事件 → `onIdle` → `onSleep`；来事件 → `onWake`。
7. `onStop()`，然后下电（预留）。

**小循环：** `loopPeriodMs()` 默认 `0`（纯事件驱动，可深睡）。返回 `N>0` 则按周期打游戏/动画帧，此时框架不自动睡。`onLoop` 必须立刻返回。阻塞的 TF / 音频 / NFC / BLE 走 `badge::jobs().post(...)`。

### 外观 — 玩法可以假定什么

公开头文件只在 `kit/include/badge/`。玩法可以假定：

- 应用代码里不会出现 GPIO、I2C 地址或 UUID。
- 钩子跑在 **App 任务**（单线程业务模型）。若有 UI 锁，只出现在 `screen().runUi(...)` 内部。
- 失败用 `badge::Status` 或 `available()==false`，不抛 C++ 异常。
- `BADGE_APP(MyClass)` 注册玩法，没有四文件 demo 表。

高阶玩法可以用小循环、`jobs()`、`power().stayAwake()`，以及以后的 `runUi`。**没有** `badge::ex::gpio()` / 裸 I2C。再往下钻，是加 Port + Adapter，或将来仍不暴露引脚的 `badge::ex`。

### 端口与适配器 — 换硅片不改玩法

| 变更 | 只动这里 |
| --- | --- |
| PCB 改脚 | `adapters/board/pinmap.hpp` |
| 同功能换芯片 | `adapters/<capability>/` |
| 新能力 | 新 Port + Adapter + 一个外观方法 — **不改** Runtime |

PCB 定稿前，`adapters/null` 实现全部 Port，host 测试和 CI 可以无板跑生命周期。

### 例子：蓝牙门面

BLE 是 **ESP32-S3 片上无线电**，不是外挂模块。门面要藏的是 NimBLE、GATT、广播间隔、MTU 和线程。玩法看见的是 **一条徽章消息**。

玩法应当能做到：开始被发现、知道连上/断开、收到一段 payload、回一句短消息、驱动灯/震动/屏幕。不应当自己选特征 UUID、ATT 操作码或连接间隔。

预期合同（骨架阶段目前只有 `radio().available()` 和 `onBleMessage`；上板后由 Adapter 填满）：

```cpp
class ChatBadge : public badge::App {
  void onStart() override { badge::radio().ble().start(); }

  void onBleConnected() override { badge::haptic().pulse(40); }

  void onBleMessage(const badge::BleMessage& m) override {
    // m.data / m.size 是业务载荷，不是 ATT 原始包
    if (m.size && m.data[0] == 0x01) {
      badge::leds().setRgb(255, 0, 0);
      badge::radio().ble().send("ok");  // 只准入队，钩子里禁止阻塞
    }
  }
};
```

| 层 | 负责 |
| --- | --- |
| 玩法 | `start` / `send` / `onBleMessage` / 连上后的 UI |
| 外观 | 默认名（`DefineJoy-Badge`）、默认服务、排队、切到 App 任务 |
| Port `IBle` | start/stop/send/connected — 不含 NimBLE 类型 |
| Adapter | 广播、GATT、MTU、功耗；默认不与 Wi-Fi 协议栈同时常驻 |

普通门面不放：UUID 表、广播 interval、PHY、配对密钥、经典蓝牙（S3 也没有）。这些留在 Adapter，或以后的 `badge::ex::ble()`。

配网和大流量走 `radio().wifi()`，不要塞进 BLE。两套栈都很吃 RAM；Adapter 应在 `start` 时再 init BLE，`stop` 时释放——和整套底座的纪律一致。

钩子里不要写 Flash、不要播很长的 PCM，请 `jobs().post(...)`。

## 硬件能力契约

下表是当前 `main` 对应用暴露的能力，不是 ESP32-S3 数据手册里的全部功能。

| 能力 | BOM / 底座已确认 | 应用接口 | 必须遵守的边界 |
| --- | --- | --- | --- |
| MCU | ESP32-S3-WROOM-1-N16R8，16 MB Flash，8 MB Octal PSRAM | ESP-IDF 目标 `esp32s3` | 不要套用通用 S3 开发板引脚 |
| 显示 | 2.4 寸屏座 TS-24CA-S26，WD3100B 背光，GT20L16S1Y 字库 | `badge::screen()` | 字库属于 Adapter 内部；面板按 ST7789 类预留，直到 BOM 单列控制器 |
| 输入 | 轻触按键 TS-1088-AR02016 | `onButton`、`badge::buttons()` | 回调禁止阻塞 |
| 震动 | CJ3401 MOSFET 驱动 | `badge::haptic()` | 马达本体可能不在贴片 BOM |
| 运动 | MPU-6050 | `onImu`、`badge::imu()` | I2C 地址只写在 `pinmap.hpp` |
| NFC | ST25DV04K I2C 动态标签 | `onNfc`、`badge::nfc()` | MCU 侧邮箱标签，不是被动 NTAG213，也不是 PN532 读卡器 |
| 灯珠 | WS2812 | `badge::leds()` | |
| 存储 | TF-01A | `badge::storage()`、`onStorage` | 路径虚拟化，应用不碰挂载细节 |
| 音频 | LMD4737 麦克风 + NS4168 功放 | `badge::audio()` | 模拟通路，**无 ES8311**；阻塞读写走 `badge::jobs()` |
| 电池 | TP5400 充电 + IP3005A 保护 | `badge::battery()` | **无电量计**；SOC 可能为 `-1` |
| 无线 | 片上 Wi-Fi / BLE | `badge::radio()`、`onBleMessage` | 只走外观；`apps/` 不要直接调 IDF |
| 电源 | Runtime Idle → Sleep → Wake | `onIdle` / `onSleep` / `onWake`、`badge::power()` | `loopPeriodMs()==0` 可自动休眠；有小循环则保持唤醒 |

引脚、I2C 地址、屏幕尺寸只在 [`adapters/board/pinmap.hpp`](adapters/board/pinmap.hpp)。BOM 对照 [`adapters/board/bom_map.md`](adapters/board/bom_map.md)。应用代码不得复制这些常量。

### 不属于当前契约的事项

不能仅凭 ESP32-S3 芯片能力推断其它板级接口。新总线或新芯片需要 Port、Adapter、外观方法和真机验收说明。见 skill [`extend-hardware`](skills/extend-hardware/SKILL.zh_CN.md)。

## 用一句需求开始开发

把类似下面的话发给 AI 助手（Cursor / Claude Code / Codex 等）：

```text
请为 DefineJoy AI Badge 开发一个离线习惯打卡应用。
使用实体按键和 2.4 寸屏，记录掉电不丢失（NVS 或 TF）。
遵守 AGENTS.md 和 docs/framework.md。
玩法放在 apps/<name>/app.cpp，用 BADGE_APP 注册，硬件只通过 badge::* 调用。
不要 include pinmap.hpp，不要写 app_main。
交付可运行玩法；若改了 Runtime 请跑 host 测试；分别报告 Build / Host tests / Device tests / Unverified。
```

建议写清：页面流程和按键事件；是否用 IMU、NFC、灯珠、震动、TF、BLE；视觉/声音目标；`loopPeriodMs` 保持 `0`（事件驱动、可休眠）还是要游戏循环。

`pinmap.hpp` 里没有的 GPIO 或寄存器，助手必须询问，禁止猜测开发板引脚。

```bash
git switch main
git switch -c feature/my-badge-app
# 实现 apps/my-badge-app/app.cpp
idf.py set-target esp32s3
idf.py -DBADGE_APP=my-badge-app build
```

## 示例

示例是设计案例，不是功能堆叠。从 `main` 起步、按需参考，不要把所有示例合并进一个固件。

| 路径 | 展示内容 |
| --- | --- |
| `apps/playground` | 最小 `onStart` + `onButton` |
| `apps/examples/buttons` | 只使用按键钩子 |
| `apps/examples/haptic` | 震动外观 |

```bash
idf.py -DBADGE_APP=playground build
idf.py -DBADGE_APP=examples/buttons build
```

## 项目结构

```text
kit/include/badge/   公开 C++ API（App 钩子 + 外观）
kit/src/             外观转发到 Ports
core/                设备 Runtime：大循环、事件泵、电源策略
ports/               无 GPIO 的接口
adapters/board/      pinmap、BOM 对照、bring-up（唯一硬件事实）
adapters/null/       PCB 定稿前的空实现
apps/                玩法；默认 playground
boot/main/           IDF 跳板（无业务）
tests/host/          不链 ESP-IDF 的生命周期测试
docs/                用户底座说明与板级事实优先级
skills/              Agent 工作流（新增玩法、扩展硬件、验收等）
AGENTS.md            AI 必读入口（配 AGENTS.zh_CN.md）
LICENSE              MIT，Copyright (c) 2026 DefineJoy
```

## 验收

Host 测试（不需要 ESP-IDF，不访问硅片）：

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

固件（需要 ESP-IDF 5.5.x）：

```bash
idf.py set-target esp32s3
idf.py -DBADGE_APP=playground build
```

交付报告：

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: 仍需板卡、仪器或用户确认的事项
```

## 文档

- [`docs/framework.md`](docs/framework.md) — 如何写玩法
- [`docs/board-facts.md`](docs/board-facts.md) — 事实来源优先级
- [`AGENTS.zh_CN.md`](AGENTS.zh_CN.md) — AI 路由（人类可跳过）
- [`skills/README.zh_CN.md`](skills/README.zh_CN.md) — Agent skills

本 README 只描述产品与仓库。AI 助手必须从 `AGENTS.md` 开始。

官网：[https://www.definejoy.cn/](https://www.definejoy.cn/) · 协议：[MIT](LICENSE) © 2026 DefineJoy
