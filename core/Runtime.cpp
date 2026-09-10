#include <core/Runtime.hpp>
#include <adapters/board/bringup.hpp>
#include <core/PortHub.hpp>
#include <core/Time.hpp>

#ifndef BADGE_HOST
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

namespace badge::core {

Runtime& Runtime::instance() {
    static Runtime rt;
    return rt;
}

void Runtime::configure(const Config& cfg) {
    cfg_ = cfg;
    power_.configure(cfg_.power);
}

void Runtime::setStayAwake(bool stay) { stay_awake_ = stay; }

void Runtime::requestSleepNow() { sleep_requested_ = true; }

void Runtime::inject(const Event& ev) { pump_.push(ev); }

void Runtime::requestStop() { running_loop_ = false; }

void Runtime::resetForTest() {
    if (app_) {
        if (state_ != LoopState::Stopped && state_ != LoopState::Booting) {
            app_->onStop();
        }
        delete app_;
        app_ = nullptr;
    }
    pump_.clear();
    jobs_.clear();
    stay_awake_ = false;
    sleep_requested_ = false;
    running_loop_ = false;
    last_loop_ms_ = 0;
    state_ = LoopState::Booting;
}

void Runtime::bringUp() {
    state_ = LoopState::Booting;
    board::bringUp();
    power_.configure(cfg_.power);

    if (!app_) {
        app_ = badge::detail::createRegisteredApp();
    }

    state_ = LoopState::Ready;
    const std::uint32_t now = Time::nowMs();
    power_.reset(now);
    last_loop_ms_ = now;
    stay_awake_ = false;
    sleep_requested_ = false;

    if (app_) app_->onStart();
    state_ = LoopState::Running;
}

void Runtime::handleEvent(const Event& ev, std::uint32_t now_ms) {
    if (state_ == LoopState::Sleeping) {
        state_ = LoopState::Waking;
        if (app_) app_->onWake();
        if (auto* p = ports().power) p->leaveSleep();
        if (auto* d = ports().display) d->wake();
        state_ = LoopState::Running;
    } else if (state_ == LoopState::Idle) {
        if (auto* d = ports().display) d->wake();
        state_ = LoopState::Running;
    }
    power_.onActivity(now_ms);
    dispatch(ev);
}

void Runtime::dispatch(const Event& ev) {
    if (!app_) return;
    switch (ev.type) {
        case EventType::Button:
            app_->onButton(ev.button, ev.button_event);
            break;
        case EventType::Nfc:
            app_->onNfc(ev.nfc);
            break;
        case EventType::Imu:
            app_->onImu(ev.imu);
            break;
        case EventType::Ble:
            app_->onBleMessage(BleMessage{nullptr, 0});
            break;
        case EventType::Storage:
            app_->onStorage(ev.storage);
            break;
    }
}

void Runtime::applyPower(std::uint32_t now_ms) {
    if (state_ != LoopState::Running && state_ != LoopState::Idle &&
        state_ != LoopState::Sleeping) {
        return;
    }

    const bool small_loop = app_ && app_->loopPeriodMs() > 0;
    if (sleep_requested_ && !stay_awake_) {
        sleep_requested_ = false;
        if (state_ != LoopState::Sleeping) {
            if (state_ == LoopState::Running && app_) app_->onIdle();
            if (app_) app_->onSleep();
            if (auto* d = ports().display) d->sleep();
            if (auto* p = ports().power) p->enterSleep();
            state_ = LoopState::Sleeping;
        }
        return;
    }

    const LoopState next =
        power_.suggest(state_, now_ms, small_loop, stay_awake_);

    if (next == LoopState::Idle && state_ == LoopState::Running) {
        if (app_) app_->onIdle();
        if (auto* d = ports().display) {
            (void)d->setBrightness(0);
        }
        state_ = LoopState::Idle;
    } else if (next == LoopState::Sleeping && state_ != LoopState::Sleeping) {
        if (state_ == LoopState::Running && app_) app_->onIdle();
        if (app_) app_->onSleep();
        if (auto* d = ports().display) d->sleep();
        if (auto* p = ports().power) p->enterSleep();
        state_ = LoopState::Sleeping;
    } else if (next == LoopState::Running && state_ == LoopState::Idle) {
        if (auto* d = ports().display) d->wake();
        state_ = LoopState::Running;
    }
}

void Runtime::step(std::uint32_t now_ms) {
    Event ev{};
    bool any = false;
    while (pump_.pop(ev)) {
        handleEvent(ev, now_ms);
        any = true;
    }

    if (!any && state_ == LoopState::Running && app_ && app_->loopPeriodMs() > 0) {
        const std::uint32_t period = app_->loopPeriodMs();
        if (now_ms - last_loop_ms_ >= period) {
            last_loop_ms_ = now_ms;
            app_->onLoop();
        }
    }

    applyPower(now_ms);
    jobs_.drain();
}

void Runtime::shutDown() {
    if (app_ && state_ != LoopState::Stopped && state_ != LoopState::Booting) {
        app_->onStop();
    }
    jobs_.drain();
    board::tearDown();
    state_ = LoopState::Stopped;
    delete app_;
    app_ = nullptr;
}

void Runtime::run() {
    running_loop_ = true;
    bringUp();
    while (running_loop_) {
        step(Time::nowMs());
#ifndef BADGE_HOST
        vTaskDelay(1);
#endif
    }
    shutDown();
}

}  // namespace badge::core
