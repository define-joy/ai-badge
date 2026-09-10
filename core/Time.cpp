#include <core/Time.hpp>
#ifndef BADGE_HOST
#include "esp_timer.h"
#endif

namespace badge::core {
namespace {
#ifdef BADGE_HOST
std::uint32_t g_now_ms = 0;
#endif
}

std::uint32_t Time::nowMs() {
#ifdef BADGE_HOST
    return g_now_ms;
#else
    return static_cast<std::uint32_t>(esp_timer_get_time() / 1000);
#endif
}

#ifdef BADGE_HOST
void Time::setNowMs(std::uint32_t ms) { g_now_ms = ms; }
void Time::advanceMs(std::uint32_t delta) { g_now_ms += delta; }
#endif

}  // namespace badge::core
