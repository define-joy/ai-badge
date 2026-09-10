#pragma once

// adapters/board/pinmap.hpp
// 本板硬件引脚与地址的【唯一事实来源】。用户 App 禁止 include 本文件。
// PCB 未定稿：所有 GPIO 均为 kNc。定稿后只改本文件，不要把数字写进 Adapter 逻辑。

#include <cstdint>

namespace badge::board {

constexpr int kNc = -1;  // 未接到 MCU / 尚未分配

// ---------------------------------------------------------------------------
// 显示：2.4 寸屏座 TS-24CA-S26，背光 WD3100B
// 面板控制器 BOM 未单列，按 ST7789 类 SPI 屏预留。
// ---------------------------------------------------------------------------
constexpr int kLcdSpiHost = 2;  // SPI2_HOST 占位，IDF 枚举在 Adapter 内转换
constexpr int kLcdMosi = kNc;
constexpr int kLcdSclk = kNc;
constexpr int kLcdCs = kNc;
constexpr int kLcdDc = kNc;
constexpr int kLcdRst = kNc;
constexpr int kLcdBl = kNc;  // 若走 WD3100B 使能脚，定稿后填
constexpr std::uint16_t kLcdWidth = 240;
constexpr std::uint16_t kLcdHeight = 320;

// GT20L16S1Y 字库（SPI，显示 Adapter 私有）
constexpr int kFontCs = kNc;

// ---------------------------------------------------------------------------
// 共享 I2C：MPU-6050 + ST25DV04K（可能还有其它从设备）
// ---------------------------------------------------------------------------
constexpr int kI2cPort = 0;
constexpr int kI2cSda = kNc;
constexpr int kI2cScl = kNc;
constexpr std::uint8_t kImuAddr = 0x68;     // MPU-6050 AD0=0
constexpr std::uint8_t kNfcAddr = 0x53;     // ST25DV 默认 E0/E1/E2 组合，定稿后核对

// ---------------------------------------------------------------------------
// 按键：TS-1088-AR02016 ×2。独立 GPIO 或 ADC 分压待原理图确认。
// ---------------------------------------------------------------------------
constexpr int kBtn0 = kNc;
constexpr int kBtn1 = kNc;
constexpr int kBtn2 = kNc;  // 预留第三键

// ---------------------------------------------------------------------------
// 震动：CJ3401 栅极
// ---------------------------------------------------------------------------
constexpr int kHapticEn = kNc;

// ---------------------------------------------------------------------------
// WS2812
// ---------------------------------------------------------------------------
constexpr int kLedData = kNc;
constexpr int kLedCount = 1;

// ---------------------------------------------------------------------------
// TF 卡：TF-01A，SDMMC 或 SPI 待原理图
// ---------------------------------------------------------------------------
constexpr int kSdCmd = kNc;
constexpr int kSdClk = kNc;
constexpr int kSdD0 = kNc;
constexpr int kSdD1 = kNc;
constexpr int kSdD2 = kNc;
constexpr int kSdD3 = kNc;
constexpr int kSdCd = kNc;

// ---------------------------------------------------------------------------
// 音频：LMD4737 模拟麦 + NS4168 功放（无 ES8311）
// ---------------------------------------------------------------------------
constexpr int kAudioPaEn = kNc;
constexpr int kAudioMicAdc = kNc;  // 若走 ADC；若走 I2S/PDM 再改名不改用户 API

// ---------------------------------------------------------------------------
// 电池：无电量计。充电状态脚待原理图。
// ---------------------------------------------------------------------------
constexpr int kChgStat = kNc;
constexpr int kBattAdc = kNc;

}  // namespace badge::board
