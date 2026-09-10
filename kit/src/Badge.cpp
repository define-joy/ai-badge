#include <badge/Badge.hpp>
#include <core/PortHub.hpp>
#include <core/Runtime.hpp>
#include <utility>

namespace badge {
namespace {

template <typename T>
bool portReady(T* p) {
    return p && p->available();
}

class Facades {
public:
    Screen screen;
    Buttons buttons;
    Haptic haptic;
    Imu imu;
    Nfc nfc;
    Leds leds;
    Storage storage;
    Audio audio;
    Battery battery;
    Radio radio;
    Jobs jobs;
    Power power;
};

Facades& facades() {
    static Facades f;
    return f;
}

}  // namespace

bool Screen::available() const { return portReady(core::ports().display); }

Status Screen::showText(const char* text) {
    auto* p = core::ports().display;
    if (!p) return Status::NotReady;
    return p->showText(text);
}

Status Screen::setBrightness(std::uint8_t percent) {
    auto* p = core::ports().display;
    if (!p) return Status::NotReady;
    return p->setBrightness(percent);
}

Status Screen::clear() {
    auto* p = core::ports().display;
    if (!p) return Status::NotReady;
    return p->clear();
}

Status Screen::runUi(const std::function<void()>& fn) {
    if (!fn) return Status::InvalidArg;
    if (!available()) return Status::NotReady;
    fn();  // 骨架：同步执行。真机切到 LVGL 任务并持锁。
    return Status::Ok;
}

bool Buttons::available() const { return portReady(core::ports().buttons); }

bool Haptic::available() const { return portReady(core::ports().haptic); }

Status Haptic::pulse(std::uint8_t strength) {
    auto* p = core::ports().haptic;
    if (!p) return Status::NotReady;
    return p->pulse(strength);
}

Status Haptic::pattern(std::uint8_t id) {
    auto* p = core::ports().haptic;
    if (!p) return Status::NotReady;
    return p->pattern(id);
}

bool Imu::available() const { return portReady(core::ports().imu); }

bool Nfc::available() const { return portReady(core::ports().nfc); }

bool Leds::available() const { return portReady(core::ports().leds); }

Status Leds::setRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) {
    auto* p = core::ports().leds;
    if (!p) return Status::NotReady;
    return p->setRgb(r, g, b);
}

Status Leds::next() {
    auto* p = core::ports().leds;
    if (!p) return Status::NotReady;
    return p->next();
}

bool Storage::available() const { return portReady(core::ports().storage); }

Status Storage::writeFile(const char* path, const void* data, std::size_t bytes) {
    auto* p = core::ports().storage;
    if (!p) return Status::NotReady;
    return p->writeFile(path, data, bytes);
}

Status Storage::readFile(const char* path, void* data, std::size_t cap, std::size_t* out_bytes) {
    auto* p = core::ports().storage;
    if (!p) return Status::NotReady;
    return p->readFile(path, data, cap, out_bytes);
}

bool Audio::available() const { return portReady(core::ports().audio); }

Status Audio::setVolume(std::uint8_t percent) {
    auto* p = core::ports().audio;
    if (!p) return Status::NotReady;
    return p->setVolume(percent);
}

bool Battery::available() const { return portReady(core::ports().battery); }

int Battery::socPercent() {
    auto* p = core::ports().battery;
    if (!p) return -1;
    return p->socPercent();
}

bool Battery::charging() {
    auto* p = core::ports().battery;
    if (!p) return false;
    return p->charging();
}

bool Radio::available() const { return portReady(core::ports().radio); }

void Jobs::post(std::function<void()> job) { core::Runtime::instance().jobs().post(std::move(job)); }

void Power::stayAwake(bool stay) { core::Runtime::instance().setStayAwake(stay); }

bool Power::stayingAwake() const { return core::Runtime::instance().stayAwake(); }

void Power::requestSleep() { core::Runtime::instance().requestSleepNow(); }

Screen& screen() { return facades().screen; }
Buttons& buttons() { return facades().buttons; }
Haptic& haptic() { return facades().haptic; }
Imu& imu() { return facades().imu; }
Nfc& nfc() { return facades().nfc; }
Leds& leds() { return facades().leds; }
Storage& storage() { return facades().storage; }
Audio& audio() { return facades().audio; }
Battery& battery() { return facades().battery; }
Radio& radio() { return facades().radio; }
Jobs& jobs() { return facades().jobs; }
Power& power() { return facades().power; }

}  // namespace badge
