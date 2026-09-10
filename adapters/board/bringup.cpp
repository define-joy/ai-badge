#include <adapters/board/bringup.hpp>
#include <adapters/board/capabilities.hpp>
#include <adapters/null/NullAdapters.hpp>
#include <core/PortHub.hpp>

namespace badge::board {
namespace {

adapters::null_impl::Display g_display;
adapters::null_impl::Buttons g_buttons;
adapters::null_impl::Haptic g_haptic;
adapters::null_impl::Imu g_imu;
adapters::null_impl::Nfc g_nfc;
adapters::null_impl::Leds g_leds;
adapters::null_impl::Storage g_storage;
adapters::null_impl::Audio g_audio;
adapters::null_impl::Battery g_battery;
adapters::null_impl::Radio g_radio;
adapters::null_impl::Power g_power;

}  // namespace

void bringUp() {
    core::PortHub hub;
#if BADGE_ADAPTER_NULL
    hub.display = &g_display;
    hub.buttons = &g_buttons;
    hub.haptic = &g_haptic;
    hub.imu = &g_imu;
    hub.nfc = &g_nfc;
    hub.leds = &g_leds;
    hub.storage = &g_storage;
    hub.audio = &g_audio;
    hub.battery = &g_battery;
    hub.radio = &g_radio;
    hub.power = &g_power;
#endif
    core::attachPorts(hub);

    auto probe = [](auto* p) {
        if (p) (void)p->probe();
    };
    probe(hub.display);
    probe(hub.buttons);
    probe(hub.haptic);
    probe(hub.imu);
    probe(hub.nfc);
    probe(hub.leds);
    probe(hub.storage);
    probe(hub.audio);
    probe(hub.battery);
    probe(hub.radio);
    probe(hub.power);
}

void tearDown() {
    auto* p = core::ports().power;
    if (p) p->leaveSleep();
}

}  // namespace badge::board
