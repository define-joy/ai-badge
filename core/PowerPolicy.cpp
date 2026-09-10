#include <core/PowerPolicy.hpp>

namespace badge::core {

void PowerPolicy::configure(const Config& cfg) { cfg_ = cfg; }

void PowerPolicy::onActivity(std::uint32_t now_ms) { last_activity_ms_ = now_ms; }

void PowerPolicy::reset(std::uint32_t now_ms) { last_activity_ms_ = now_ms; }

LoopState PowerPolicy::suggest(LoopState current,
                               std::uint32_t now_ms,
                               bool small_loop_active,
                               bool stay_awake) const {
    if (stay_awake || small_loop_active) {
        return LoopState::Running;
    }
    const std::uint32_t idle_at = last_activity_ms_ + cfg_.idle_timeout_ms;
    const std::uint32_t sleep_at = idle_at + cfg_.sleep_timeout_ms;

    if (now_ms >= sleep_at) return LoopState::Sleeping;
    if (now_ms >= idle_at) return LoopState::Idle;

    if (current == LoopState::Idle || current == LoopState::Sleeping ||
        current == LoopState::Waking) {
        return current;
    }
    return LoopState::Running;
}

}  // namespace badge::core
