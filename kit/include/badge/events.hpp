#pragma once

#include <cstdint>
#include <cstddef>

namespace badge {

enum class Button : std::uint8_t {
    Up = 0,
    Down,
    Ok,
};

enum class ButtonEvent : std::uint8_t {
    Press = 0,
    Click,
    Double,
    Long,
};

enum class ImuEventType : std::uint8_t {
    Shake = 0,
    Orientation,
};

struct ImuEvent {
    ImuEventType type;
    float ax;
    float ay;
    float az;
};

struct NfcEvent {
    std::uint8_t uid[8];
    std::uint8_t uid_len;
};

struct BleMessage {
    const std::uint8_t* data;
    std::size_t size;
};

enum class StorageEventType : std::uint8_t {
    Mounted = 0,
    Unmounted,
    Error,
};

struct StorageEvent {
    StorageEventType type;
};

enum class EventType : std::uint8_t {
    Button = 0,
    Nfc,
    Imu,
    Ble,
    Storage,
};

struct Event {
    EventType type{};
    Button button{};
    ButtonEvent button_event{};
    ImuEvent imu{};
    NfcEvent nfc{};
    StorageEvent storage{};
};

}  // namespace badge
