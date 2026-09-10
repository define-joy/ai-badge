#pragma once

// 编译期「这块板打算有什么」。运行时是否在线看 Port::probe() / available()。

namespace badge::board {

struct Capabilities {
    static constexpr bool display = true;
    static constexpr bool buttons = true;
    static constexpr bool haptic = true;
    static constexpr bool imu = true;
    static constexpr bool nfc = true;
    static constexpr bool leds = true;
    static constexpr bool storage = true;
    static constexpr bool audio = true;
    static constexpr bool battery = true;   // 充电/保护有，精确 SOC 无
    static constexpr bool fuel_gauge = false;
    static constexpr bool radio = true;
    static constexpr bool font_rom = true;  // GT20L16S1Y，非用户 Port
};

// 骨架阶段一律走 null adapter；PCB 通电后改为 0 并链接真实 Adapter。
#ifndef BADGE_ADAPTER_NULL
#define BADGE_ADAPTER_NULL 1
#endif

}  // namespace badge::board
