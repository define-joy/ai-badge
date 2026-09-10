#pragma once

#include <badge/Status.hpp>
#include <cstddef>
#include <cstdint>

namespace badge::ports {

class IDisplay {
public:
    virtual ~IDisplay() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    virtual Status showText(const char* text) = 0;
    virtual Status setBrightness(std::uint8_t percent) = 0;
    virtual Status clear() = 0;
    virtual void sleep() = 0;
    virtual void wake() = 0;
};

class IButtons {
public:
    virtual ~IButtons() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
};

class IHaptic {
public:
    virtual ~IHaptic() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    virtual Status pulse(std::uint8_t strength) = 0;
    virtual Status pattern(std::uint8_t id) = 0;
};

class IImu {
public:
    virtual ~IImu() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
};

class INfc {
public:
    virtual ~INfc() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
};

class ILeds {
public:
    virtual ~ILeds() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    virtual Status setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) = 0;
    virtual Status next() = 0;
    virtual void sleep() = 0;
};

class IStorage {
public:
    virtual ~IStorage() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    virtual Status writeFile(const char* path, const void* data, std::size_t bytes) = 0;
    virtual Status readFile(const char* path, void* data, std::size_t cap, std::size_t* out_bytes) = 0;
};

class IAudio {
public:
    virtual ~IAudio() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    virtual Status setVolume(std::uint8_t percent) = 0;
};

class IBattery {
public:
    virtual ~IBattery() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
    // 0..100；读失败返回 -1。本板无电量计时可能一直为 -1。
    virtual int socPercent() = 0;
    virtual bool charging() = 0;
};

class IRadio {
public:
    virtual ~IRadio() = default;
    virtual Status probe() = 0;
    virtual bool available() const = 0;
};

class IPower {
public:
    virtual ~IPower() = default;
    virtual Status probe() = 0;
    virtual void enterSleep() = 0;
    virtual void leaveSleep() = 0;
};

}  // namespace badge::ports
