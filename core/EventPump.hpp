#pragma once

#include <badge/events.hpp>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <mutex>

namespace badge::core {

class EventPump {
public:
    bool push(const Event& ev);
    bool pop(Event& out);
    void clear();
    std::size_t size() const;

private:
    mutable std::mutex mu_;
    std::deque<Event> q_;
};

}  // namespace badge::core
