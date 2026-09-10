# adapters

硬件适配层，用户玩法不要依赖本目录。

- `board/pinmap.hpp`：引脚与 I2C 地址的唯一来源（现为占位）
- `board/capabilities.hpp`：本板有哪些外设
- `board/bom_map.md`：BOM 芯片到 Port 的对照
- `board/bringup.cpp`：启动时挂上 Adapter 并 probe
- `null/`：无硬件空实现，保证 host 测试与未焊 PCB 时也能跑通生命周期

PCB 通电后：把 `BADGE_ADAPTER_NULL` 改为 0，在 `display/` `imu/` `nfc/` 等目录加入真实驱动，并在 `bringup.cpp` 里替换指针。Port 与 `badge::App` 保持不变。
