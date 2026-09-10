#pragma once

#include <badge/App.hpp>
#include <core/EventPump.hpp>
#include <core/JobQueue.hpp>
#include <core/PowerPolicy.hpp>
#include <cstdint>

namespace badge::core {

class Runtime {
public:
    struct Config {
        PowerPolicy::Config power{};
    };

    static Runtime& instance();

    void configure(const Config& cfg);
    void bringUp();
    void step(std::uint32_t now_ms);
    void requestStop();
    void shutDown();
    void run();  // 设备：bringUp + 循环 step，直到 requestStop

    void inject(const Event& ev);
    LoopState state() const { return state_; }
    App* app() const { return app_; }
    bool stayAwake() const { return stay_awake_; }
    void setStayAwake(bool stay);
    void requestSleepNow();
    JobQueue& jobs() { return jobs_; }
    EventPump& pump() { return pump_; }

    // 测试用：丢掉 App/状态，不释放 Port（null adapter 是静态的）
    void resetForTest();

private:
    Runtime() = default;
    void handleEvent(const Event& ev, std::uint32_t now_ms);
    void dispatch(const Event& ev);
    void applyPower(std::uint32_t now_ms);

    Config cfg_{};
    LoopState state_ = LoopState::Booting;
    App* app_ = nullptr;
    EventPump pump_;
    JobQueue jobs_;
    PowerPolicy power_;
    bool stay_awake_ = false;
    bool sleep_requested_ = false;
    bool running_loop_ = false;
    std::uint32_t last_loop_ms_ = 0;
};

}  // namespace badge::core
