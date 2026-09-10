# DefineJoy AI Badge

[English](README.md) | [简体中文](README.zh_CN.md)

DefineJoy AI Badge is open wearable AI hardware. This repository is the **user development kit**: hardware facts, a stable Facade API, a fixed device lifecycle, examples, and validation methods live in one place. Application authors write `badge::App` hooks. They do not write GPIO, `app_main`, or driver bring-up.

**Hardware samples are available.** Apply on the official website: [https://www.definejoy.cn/](https://www.definejoy.cn/)

The repository is organized around these principles:

- `main` is the smallest complete baseline and an executable description of the current hardware contract.
- `kit/include/badge/` is the only public API. Pins and chips stay in `adapters/`.
- `apps/` holds plays. Register a play with `BADGE_APP`; do not maintain a central demo table.
- AI assistants start at [`AGENTS.md`](AGENTS.md). Board facts: [`docs/board-facts.md`](docs/board-facts.md). User guide: [`docs/framework.md`](docs/framework.md).
- Build results and physical-device results are reported separately. A successful compile is never hardware validation.

PCB routing is not frozen. GPIO values in `adapters/board/pinmap.hpp` are placeholders (`-1`). Current adapters may be null stubs; `available()` and host tests do not prove silicon.

## Design

The kit combines **Template Method** (fixed device lifecycle, user-filled hooks) and **Facade** (hardware behind `badge::*`). In embedded terms this is also **Ports & Adapters**: plays depend on interfaces, not on GPIO or NimBLE. Arduino `setup()` / `loop()` is the same family of idea; here the skeleton also owns the event pump and power policy.

```text
Play (apps/)
  hooks: onStart / onLoop / onButton / onBleMessage / ...
  calls: badge::screen() haptic() radio().ble() ...
        ↓  Facade — no pins, no driver types
Ports (ports/)     GPIO-free contracts
        ↓
Adapters           chips, pinmap, ESP-IDF, NimBLE
        ↑
Runtime (core/)    big loop the play cannot override
```

### Template Method — big loop and small loop

`core::Runtime` is the algorithm skeleton. Plays do not write `app_main` and do not choose init order.

1. Bring up buses once (idempotent).
2. Probe peripherals. One failure does not halt the device (`available()==false`).
3. Driver callbacks only **enqueue** events. They must not call user virtuals from ISR or the BLE task.
4. `onStart()`.
5. Event pump: dispatch hooks; if `loopPeriodMs()>0` and there is no event, call `onLoop()`.
6. Power: no small loop and no events → `onIdle` → `onSleep`; an event → `onWake`.
7. `onStop()`, then teardown (reserved).

**Small loop:** `loopPeriodMs()` defaults to `0` (event-driven, may deep-sleep). Return `N>0` for a game/animation tick; the framework then will not auto-sleep. `onLoop` must return immediately. Blocking TF / audio / NFC / BLE I/O goes to `badge::jobs().post(...)`.

### Facade — what the play is allowed to assume

Public headers live only in `kit/include/badge/`. A play may assume:

- No GPIO, I2C address, or UUID appears in application code.
- Hooks run on the **App task** (single-threaded business model). UI locking, if any, stays inside `screen().runUi(...)`.
- Failures are `badge::Status` or `available()==false`, not C++ exceptions.
- `BADGE_APP(MyClass)` registers the play. There is no four-file demo table.

Advanced plays may use `loopPeriodMs`, `jobs()`, `power().stayAwake()`, and later `screen().runUi`. There is **no** `badge::ex::gpio()` / raw I2C. Experts extend **Ports + Adapters**, or a future `badge::ex` that still hides pins.

### Ports & Adapters — changing silicon without changing plays

| Change | Touch only |
| --- | --- |
| PCB pin swap | `adapters/board/pinmap.hpp` |
| Same function, new chip | `adapters/<capability>/` |
| New capability | new Port + Adapter + one Facade method — **not** Runtime |

Until PCB freeze, `adapters/null` implements every Port so host tests and CI can run the lifecycle with no board.

### Example: Bluetooth Facade

BLE is the **ESP32-S3 on-chip radio**, not a discrete module. The Facade hides NimBLE, GATT, advertising intervals, MTU, and threads. The play sees a **badge message**.

What a play should be able to do: start being discoverable, learn connect/disconnect, receive a payload, send a short reply, drive LEDs/haptic/screen. It should not choose characteristic UUIDs, ATT opcodes, or connection intervals.

Intended contract (skeleton today only has `radio().available()` and `onBleMessage`; the Adapter will fill this in after bring-up):

```cpp
class ChatBadge : public badge::App {
  void onStart() override { badge::radio().ble().start(); }

  void onBleConnected() override { badge::haptic().pulse(40); }

  void onBleMessage(const badge::BleMessage& m) override {
    // m.data / m.size is the play payload, not an ATT packet
    if (m.size && m.data[0] == 0x01) {
      badge::leds().setRgb(255, 0, 0);
      badge::radio().ble().send("ok");  // enqueue only; hook must not block
    }
  }
};
```

| Layer | Owns |
| --- | --- |
| Play | `start` / `send` / `onBleMessage` / connected UI |
| Facade | default name (`DefineJoy-Badge`), default service, queue, hop to App task |
| Port `IBle` | start/stop/send/connected — no NimBLE types |
| Adapter | advertise, GATT, MTU, power; stacks not resident together with Wi-Fi unless required |

Stay out of the normal Facade: UUID tables, advertise interval, PHY, pairing secrets, Classic Bluetooth (the S3 has none). Those belong in Adapter or a later `badge::ex::ble()`.

Wi-Fi provisioning and bulk transfer belong on `radio().wifi()`, not on BLE. Both stacks are RAM-heavy; the Adapter should init BLE on demand and release it on `stop()`, the same discipline as the rest of the kit.

Hooks must not write Flash or play long PCM. Post that work with `jobs()`.

## Hardware capability contract

The table is what this `main` branch exposes to applications, not every feature in the ESP32-S3 datasheet.

| Capability | Confirmed on BOM / kit | Application interface | Boundaries |
| --- | --- | --- | --- |
| MCU | ESP32-S3-WROOM-1-N16R8, 16 MB Flash, 8 MB octal PSRAM | ESP-IDF target `esp32s3` | Do not copy generic S3-DevKit pinouts |
| Display | 2.4" panel (TS-24CA-S26), WD3100B backlight, GT20L16S1Y font ROM | `badge::screen()` | Font ROM is adapter-private; LCD controller is reserved as ST7789-class until the panel BOM line is explicit |
| Input | Tactile keys (TS-1088-AR02016) | `onButton`, `badge::buttons()` | Callbacks must not block |
| Haptic | CJ3401 MOSFET drive | `badge::haptic()` | Motor part may sit outside the SMT BOM |
| Motion | MPU-6050 | `onImu`, `badge::imu()` | I2C address lives only in `pinmap.hpp` |
| NFC | ST25DV04K I2C dynamic tag | `onNfc`, `badge::nfc()` | MCU mailbox tag, not a passive NTAG213 and not a PN532 reader |
| LEDs | WS2812 | `badge::leds()` | |
| Storage | TF-01A MicroSD | `badge::storage()`, `onStorage` | Mount point is virtualized |
| Audio | LMD4737 mic + NS4168 amp | `badge::audio()` | Analog path; **no ES8311**. PCM-style work belongs in `badge::jobs()` |
| Battery | TP5400 charge + IP3005A protect | `badge::battery()` | **No fuel gauge**; SOC may be `-1` |
| Radio | On-chip Wi-Fi / BLE | `badge::radio()`, `onBleMessage` | Facade only; do not call IDF Wi-Fi/NimBLE from `apps/` |
| Power | Runtime Idle → Sleep → Wake | `onIdle` / `onSleep` / `onWake`, `badge::power()` | `loopPeriodMs()==0` allows auto-sleep; a small loop stays awake |

Pins, I2C addresses, and panel size exist only in [`adapters/board/pinmap.hpp`](adapters/board/pinmap.hpp). BOM → Port map: [`adapters/board/bom_map.md`](adapters/board/bom_map.md). Application code must not duplicate those constants.

### Outside the current contract

Do not infer extra board interfaces from the ESP32-S3 feature list. A new bus or chip needs a Port, an Adapter, a Facade method, and an on-device acceptance note. Skill: [`skills/extend-hardware`](skills/extend-hardware/SKILL.md).

## Start development with one requirement

Paste a request like this into an AI assistant (Cursor, Claude Code, Codex, …):

```text
Build an offline habit-tracking app for DefineJoy AI Badge.
Use the physical buttons and the 2.4" display. Keep records across power loss (NVS or TF).
Follow AGENTS.md and docs/framework.md.
Put play logic in apps/<name>/app.cpp, register it with BADGE_APP, and call hardware only through badge::*.
Do not include pinmap.hpp or write app_main.
Deliver a runnable play, host tests if you touch Runtime, and report Build / Host tests / Device tests / Unverified separately.
```

Useful details: page flow and button events; whether you need IMU, NFC, LEDs, haptic, TF, or BLE; visual/audio goals; whether `loopPeriodMs` should stay `0` (event-driven, can sleep) or run a game loop.

If a GPIO or register is missing from `pinmap.hpp`, the assistant must ask. It must not guess a DevKit pin.

```bash
git switch main
git switch -c feature/my-badge-app
# implement apps/my-badge-app/app.cpp
idf.py set-target esp32s3
idf.py -DBADGE_APP=my-badge-app build
```

## Examples

Examples are design cases, not a feature pile. Start from `main` and copy patterns; do not merge every example into one firmware.

| Path | What it shows |
| --- | --- |
| `apps/playground` | Minimal `onStart` + `onButton` |
| `apps/examples/buttons` | Button hooks only |
| `apps/examples/haptic` | Vibration Facade |

```bash
idf.py -DBADGE_APP=playground build
idf.py -DBADGE_APP=examples/buttons build
```

## Project structure

```text
kit/include/badge/   Public C++ API (App hooks + Facade)
kit/src/             Facade forwarding to Ports
core/                Device Runtime: big loop, EventPump, PowerPolicy
ports/               GPIO-free interfaces
adapters/board/      pinmap, BOM map, bring-up (the only hardware facts)
adapters/null/       Host/stub adapters until PCB freeze
apps/                Plays; default playground
boot/main/           IDF trampoline only (no product logic)
tests/host/          Lifecycle tests without ESP-IDF
docs/                User kit + board-fact priority
skills/              Agent workflows (add-play, extend-hardware, validate, …)
AGENTS.md            Mandatory AI entry (paired AGENTS.zh_CN.md)
LICENSE              MIT, Copyright (c) 2026 DefineJoy
```

## Validation

Host tests (no ESP-IDF, no silicon):

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

Firmware (needs ESP-IDF 5.5.x):

```bash
idf.py set-target esp32s3
idf.py -DBADGE_APP=playground build
```

Delivery report:

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: remaining board, instrument, or user checks
```

## Documentation

- [`docs/framework.md`](docs/framework.md) — how to write a play
- [`docs/board-facts.md`](docs/board-facts.md) — source-of-truth order
- [`AGENTS.md`](AGENTS.md) — AI routing (humans may skip)
- [`skills/README.md`](skills/README.md) — agent skills

This README describes the product and the repository. AI agents must start from `AGENTS.md`.

Website: [https://www.definejoy.cn/](https://www.definejoy.cn/) · License: [MIT](LICENSE) © 2026 DefineJoy
