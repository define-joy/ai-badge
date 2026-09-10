#pragma once

#include <cstdint>

namespace badge::core {

enum class LoopState : std::uint8_t {
    Booting = 0,
    Ready,
    Running,
    Idle,
    Sleeping,
    Waking,
    Stopped,
};

class PowerPolicy {
public:
    struct Config {
        std::uint32_t idle_timeout_ms = 15000;
        std::uint32_t sleep_timeout_ms = 15000;
    };

    void configure(const Config& cfg);
    void onActivity(std::uint32_t now_ms);
    void reset(std::uint32_t now_ms);

    LoopState suggest(LoopState current,
                      std::uint32_t now_ms,
                      bool small_loop_active,
                      bool stay_awake) const;

private:
    Config cfg_{};
    std::uint32_t last_activity_ms_ = 0;
};

}  // namespace badge::core
