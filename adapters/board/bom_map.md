# BOM → Port 映射

来源：`e:\乐哉科技\BOM_V2.0_V2.0_2026-09-08.xlsx`（V2.0，2026-09-08）。
GPIO 尚未随 PCB 定稿，数值只存在 [`pinmap.hpp`](pinmap.hpp)，且全部为占位。

用户 API 不得出现料号、封装或引脚。换芯片只改对应 Adapter。

## 主控

| BOM | 规格 | 框架落点 |
| --- | --- | --- |
| ESP32-S3-WROOM-1-N16R8 | 16 MB Flash，8 MB Octal PSRAM | `platform/sdkconfig.defaults` |

## 能力映射

| Port | BOM 器件 | 说明 |
| --- | --- | --- |
| IDisplay | TS-24CA-S26（2.4 寸屏座）+ WD3100B 背光恒流 | 面板驱动芯片 BOM 未单列，按 2.4 寸常见 ST7789 预留 Adapter 槽；字库 GT20L16S1Y 属于显示适配层内部，不暴露给用户 |
| IButtons | TS-1088-AR02016（×2） | 轻触按键；键位语义（上/下/确定等）等原理图后再写入 pinmap |
| IHaptic | CJ3401 MOSFET | 典型震动马达开关管；马达本体可能不在本 BOM |
| IImu | MPU-6050 | 六轴陀螺仪/加速度计，I2C |
| INfc | ST25DV04K-IER6S3 | MCU 侧 I2C 动态标签（邮箱），不是被动 NTAG213，也不是 PN532 读卡器 |
| ILeds | WS2812 | RGB 灯珠 |
| IStorage | TF-01A | MicroSD 卡座 |
| IAudio | LMD4737 麦克风 + NS4168 功放 | **无 ES8311**；模拟麦 + 模拟功放，与竞品 codec 路径不同 |
| IBattery | TP5400 充电 + IP3005A 保护 | **无电量计**；SOC 暂用充电器状态/ADC 占位，`available()` 可先为弱能力 |
| IRadio | ESP32-S3 片上 Wi-Fi / BLE | 无线在 Facade 内封装，用户不直接调 IDF |

## 明确不进入用户 Port 的器件

- GT20L16S1Y 中文字库：显示 Adapter 私有
- WD3100B 背光恒流：显示 Adapter 私有
- SY8088 / AMS1117 / TP5400 / IP3005A 电源树：板级 bringup 私有
- USB ESD、Type-C 座、晶振/ESD：硬件事实，无软件 API

## 换件规则

1. 改引脚 → 只改 `pinmap.hpp`
2. 换同功能芯片 → 只换 `adapters/<capability>/` 实现
3. 新增能力 → 新 Port + Adapter + `badge::` 外观方法，不改 Runtime
