#pragma once

#include <cstdint>

namespace badge::core {

class Time {
public:
    static std::uint32_t nowMs();
#ifdef BADGE_HOST
    static void setNowMs(std::uint32_t ms);
    static void advanceMs(std::uint32_t delta);
#endif
};

}  // namespace badge::core
