# 板级事实优先级

给后续在本仓库工作的开发者与 AI 助手。发生冲突时按下面顺序，**不要用通用 ESP32-S3 开发板参数填空**。

```text
产品规格 / BOM / 实机测量
    > adapters/board/pinmap.hpp
    > adapters/board/capabilities.hpp 与 Adapter 实现
    > adapters/board/bom_map.md
    > docs/framework.md 与示例 App
```

引脚或寄存器未在上述来源出现时，停下来问人，不要猜 GPIO。

## 当前已确认

- MCU：ESP32-S3-WROOM-1-N16R8（16MB Flash，8MB Octal PSRAM）
- 屏：2.4 寸（座子 TS-24CA-S26），背光 WD3100B
- 运动：MPU-6050
- NFC：ST25DV04K（I2C 动态标签）
- 灯：WS2812
- 存储：TF-01A
- 音频：LMD4737 麦克风 + NS4168 功放（无 ES8311）
- 电池：TP5400 + IP3005A（无 CW2017 电量计）
- 震动驱动：CJ3401

GPIO 在 `pinmap.hpp` 里仍是占位 `-1`，等 PCB 定稿。

## 分层

- 用户只改 `apps/`
- 稳定 API 只在 `kit/include/badge/`
- 大循环在 `core/`
- 硬件只在 `adapters/`
