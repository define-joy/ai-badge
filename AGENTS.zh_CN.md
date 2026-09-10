<p align="right">
  <strong>简体中文</strong> · <a href="AGENTS.md">English</a>
</p>

# AI 助手仓库指南

本文件是本仓库 AI 协助开发的唯一强制入口。按下面的路由表加载任务文档，不要默认读完所有 README。

## 项目与安全基线

- 品牌：**DefineJoy**。产品固件：**AI Badge**。
- 目标芯片：ESP32-S3-WROOM-1-N16R8，16 MB Flash，8 MB Octal PSRAM。上板构建使用 ESP-IDF 5.5.x。
- 用户代码只放 `apps/`。稳定 API 只在 `kit/include/badge/`。引脚、总线、芯片驱动只在 `adapters/`，不得泄漏进玩法。
- 硬件事实优先级：产品规格 / BOM / 实机测量 → `adapters/board/pinmap.hpp` → Adapter 实现 → `adapters/board/bom_map.md` → `docs/framework.md`。GPIO、地址、寄存器未定义时，询问用户，不要套用通用 ESP32-S3 开发板。
- PCB 通电前 `BADGE_ADAPTER_NULL` 为 1。空实现可以返回 `Ok` 但不代表硅片在线。禁止把 host 测试或固件**编译通过**说成真机验收。
- 保留用户已有改动。先执行 `git status --short --branch`，不要覆盖无关文件。
- 钩子（`onButton`、`onLoop` 等）禁止阻塞。音频、TF、NFC 等慢路径走 `badge::jobs().post(...)`。
- `apps/` 里不要 include `pinmap.hpp`、ESP-IDF 驱动头或 LVGL 锁。
- 禁止提交凭证、设备密钥、私钥、个人数据或未脱敏日志。
- 协议为 MIT，Copyright (c) 2026 DefineJoy。复制实质性内容时保留 `LICENSE`。

## 任务路由

| 任务 | 修改前阅读 |
| --- | --- |
| 任意代码 | `docs/framework.md`，相邻头文件 |
| 引脚、总线、芯片、BOM | `docs/board-facts.md`，`adapters/board/pinmap.hpp`，`adapters/board/bom_map.md` |
| 玩法 / 示例 | `apps/README.md`，`kit/include/badge/App.hpp`，最近的 `apps/**/app.cpp` |
| 运行时 / 电源 / 事件 | `core/Runtime.hpp`，`docs/framework.md` |
| 新外设 | skill `extend-hardware` |
| 新玩法 | skill `add-play` |
| Host 测试 | `tests/host/README.md`，skill `validate-framework` |
| 提交或 PR | 英文祈使句 Conventional Commit；未经用户要求不要 push |

## 验收与交付

迭代时跑最小相关检查。C++ 逻辑变更后交付前跑 host 测试：

```bash
cmake -S tests/host -B tests/host/build
cmake --build tests/host/build
./tests/host/build/badge_host_tests
```

设备固件（`idf.py set-target esp32s3` 然后 `idf.py -DBADGE_APP=playground build`）是另一道门禁，需要 ESP-IDF。交付时分开报告：

```text
Build: PASS / FAIL / NOT RUN
Host tests: PASS / FAIL / NOT RUN
Device tests: PASS / FAIL / NOT RUN
Unverified: 仍需板卡、仪器或用户确认的事项
```

仅在用户明确要求时创建 commit 或 push。
