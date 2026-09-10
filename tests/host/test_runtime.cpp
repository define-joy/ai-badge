#include <badge/App.hpp>
#include <badge/Badge.hpp>
#include <core/Runtime.hpp>
#include <core/Time.hpp>

#include <cstdio>
#include <string>
#include <vector>

using badge::core::LoopState;
using badge::core::Runtime;
using badge::core::Time;

namespace {

int g_fails = 0;
int g_checks = 0;

void check(bool cond, const char* expr, const char* file, int line) {
    ++g_checks;
    if (!cond) {
        ++g_fails;
        std::fprintf(stderr, "FAIL %s:%d  %s\n", file, line, expr);
    }
}

#define CHECK(expr) check(static_cast<bool>(expr), #expr, __FILE__, __LINE__)

std::vector<std::string> g_log;
int g_loops = 0;

void log(const char* s) { g_log.emplace_back(s); }

class DispatchApp : public badge::App {
public:
    void onStart() override { log("start"); }
    void onLoop() override {
        log("loop");
        ++g_loops;
    }
    void onStop() override { log("stop"); }
    void onButton(badge::Button, badge::ButtonEvent) override { log("button"); }
    void onNfc(const badge::NfcEvent&) override { log("nfc"); }
    void onIdle() override { log("idle"); }
    void onSleep() override { log("sleep"); }
    void onWake() override { log("wake"); }
};

class NoLoopApp : public DispatchApp {
public:
    std::uint32_t loopPeriodMs() const override { return 0; }
};

class TickApp : public DispatchApp {
public:
    std::uint32_t loopPeriodMs() const override { return 10; }
};

badge::App* make_no_loop() { return new NoLoopApp(); }
badge::App* make_tick() { return new TickApp(); }

Runtime& rt() { return Runtime::instance(); }

void setup(badge::detail::AppFactory factory, std::uint32_t idle_ms, std::uint32_t sleep_ms) {
    rt().resetForTest();
    g_log.clear();
    g_loops = 0;
    Time::setNowMs(0);
    Runtime::Config cfg;
    cfg.power.idle_timeout_ms = idle_ms;
    cfg.power.sleep_timeout_ms = sleep_ms;
    rt().configure(cfg);
    badge::detail::registerAppFactory(factory);
    rt().bringUp();
}

void test_dispatch_order() {
    setup(&make_no_loop, 1000, 1000);
    CHECK(rt().state() == LoopState::Running);
    CHECK(!g_log.empty() && g_log[0] == "start");

    badge::Event button{};
    button.type = badge::EventType::Button;
    button.button = badge::Button::Ok;
    button.button_event = badge::ButtonEvent::Click;
    rt().inject(button);

    badge::Event nfc{};
    nfc.type = badge::EventType::Nfc;
    nfc.nfc.uid_len = 4;
    rt().inject(nfc);

    rt().step(0);
    CHECK(g_log.size() >= 3);
    CHECK(g_log[1] == "button");
    CHECK(g_log[2] == "nfc");
    rt().shutDown();
}

void test_no_small_loop() {
    setup(&make_no_loop, 10000, 10000);
    CHECK(g_loops == 0);
    rt().step(10);
    rt().step(20);
    rt().step(50);
    CHECK(g_loops == 0);
    for (const auto& s : g_log) {
        CHECK(s != "loop");
    }
    rt().shutDown();
}

void test_small_loop_ticks() {
    setup(&make_tick, 10000, 10000);
    rt().step(0);
    CHECK(g_loops == 0);
    rt().step(10);
    CHECK(g_loops == 1);
    rt().step(20);
    CHECK(g_loops == 2);
    rt().shutDown();
}

void test_idle_then_sleep_then_wake() {
    setup(&make_no_loop, 50, 50);
    CHECK(rt().state() == LoopState::Running);

    rt().step(50);
    CHECK(rt().state() == LoopState::Idle);
    CHECK(g_log.back() == "idle");

    rt().step(100);
    CHECK(rt().state() == LoopState::Sleeping);
    CHECK(g_log.back() == "sleep");

    badge::Event button{};
    button.type = badge::EventType::Button;
    button.button_event = badge::ButtonEvent::Press;
    rt().inject(button);
    rt().step(100);
    CHECK(rt().state() == LoopState::Running);

    bool saw_wake = false;
    bool saw_button_after_wake = false;
    bool after_wake = false;
    for (const auto& s : g_log) {
        if (s == "wake") {
            saw_wake = true;
            after_wake = true;
        } else if (after_wake && s == "button") {
            saw_button_after_wake = true;
        }
    }
    CHECK(saw_wake);
    CHECK(saw_button_after_wake);
    rt().shutDown();
}

void test_small_loop_blocks_sleep() {
    setup(&make_tick, 50, 50);
    rt().step(50);
    rt().step(100);
    CHECK(rt().state() == LoopState::Running);
    rt().shutDown();
}

void test_facade_after_bringup() {
    setup(&make_no_loop, 1000, 1000);
    CHECK(badge::screen().available());
    CHECK(badge::haptic().available());
    CHECK(badge::ok(badge::screen().showText("hi")));
    CHECK(badge::ok(badge::haptic().pulse(40)));
    CHECK(badge::battery().socPercent() == -1);
    rt().shutDown();
}

}  // namespace

int main() {
    test_dispatch_order();
    test_no_small_loop();
    test_small_loop_ticks();
    test_idle_then_sleep_then_wake();
    test_small_loop_blocks_sleep();
    test_facade_after_bringup();

    std::printf("host tests: %d checks, %d failed\n", g_checks, g_fails);
    return g_fails == 0 ? 0 : 1;
}
