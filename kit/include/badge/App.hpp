#pragma once

#include <badge/events.hpp>
#include <cstdint>

namespace badge {

class App {
public:
    virtual ~App() = default;

    virtual void onStart() {}
    virtual void onLoop() {}
    virtual void onStop() {}

    // 返回 0：关闭小循环（纯事件驱动，允许自动休眠）。
    // 返回 N>0：每 N 毫秒在无事件时调用 onLoop()；有小循环时框架不自动进 Sleep。
    // onLoop 必须非阻塞。阻塞工作请 badge::jobs().post(...)。
    virtual std::uint32_t loopPeriodMs() const { return 0; }

    virtual void onButton(Button, ButtonEvent) {}
    virtual void onNfc(const NfcEvent&) {}
    virtual void onImu(const ImuEvent&) {}
    virtual void onBleMessage(const BleMessage&) {}
    virtual void onStorage(const StorageEvent&) {}
    virtual void onIdle() {}
    virtual void onSleep() {}
    virtual void onWake() {}
};

namespace detail {
using AppFactory = App* (*)();
void registerAppFactory(AppFactory factory);
App* createRegisteredApp();
}  // namespace detail

}  // namespace badge

#define BADGE_APP(Type)                                                          \
    namespace {                                                                  \
    struct BadgeAppRegistrar {                                                   \
        BadgeAppRegistrar() {                                                    \
            badge::detail::registerAppFactory([]() -> badge::App* {              \
                return new Type();                                               \
            });                                                                  \
        }                                                                        \
    };                                                                           \
    static BadgeAppRegistrar badge_app_registrar;                                \
    }
