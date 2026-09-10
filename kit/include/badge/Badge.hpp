#pragma once

#include <badge/Status.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>

namespace badge {

class Screen {
public:
    bool available() const;
    Status showText(const char* text);
    Status setBrightness(std::uint8_t percent);
    Status clear();
    // 进阶：在 UI 任务上下文执行。骨架阶段同步调用；真机将持有 LVGL 锁。
    Status runUi(const std::function<void()>& fn);
};

class Buttons {
public:
    bool available() const;
};

class Haptic {
public:
    bool available() const;
    Status pulse(std::uint8_t strength);
    Status pattern(std::uint8_t id);
};

class Imu {
public:
    bool available() const;
};

class Nfc {
public:
    bool available() const;
};

class Leds {
public:
    bool available() const;
    Status setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b);
    Status next();
};

class Storage {
public:
    bool available() const;
    Status writeFile(const char* path, const void* data, std::size_t bytes);
    Status readFile(const char* path, void* data, std::size_t cap, std::size_t* out_bytes);
};

class Audio {
public:
    bool available() const;
    Status setVolume(std::uint8_t percent);
};

class Battery {
public:
    bool available() const;
    int socPercent();
    bool charging();
};

class Radio {
public:
    bool available() const;
};

class Jobs {
public:
    void post(std::function<void()> job);
};

class Power {
public:
    void stayAwake(bool stay);
    bool stayingAwake() const;
    void requestSleep();
};

Screen& screen();
Buttons& buttons();
Haptic& haptic();
Imu& imu();
Nfc& nfc();
Leds& leds();
Storage& storage();
Audio& audio();
Battery& battery();
Radio& radio();
Jobs& jobs();
Power& power();

}  // namespace badge
