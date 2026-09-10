#pragma once

#include <ports/Interfaces.hpp>

namespace badge::core {

struct PortHub {
    ports::IDisplay* display = nullptr;
    ports::IButtons* buttons = nullptr;
    ports::IHaptic* haptic = nullptr;
    ports::IImu* imu = nullptr;
    ports::INfc* nfc = nullptr;
    ports::ILeds* leds = nullptr;
    ports::IStorage* storage = nullptr;
    ports::IAudio* audio = nullptr;
    ports::IBattery* battery = nullptr;
    ports::IRadio* radio = nullptr;
    ports::IPower* power = nullptr;
};

PortHub& ports();
void attachPorts(const PortHub& hub);

}  // namespace badge::core
