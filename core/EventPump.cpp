#include <core/EventPump.hpp>

namespace badge::core {

bool EventPump::push(const Event& ev) {
    std::lock_guard<std::mutex> lock(mu_);
    if (q_.size() >= 32) return false;
    q_.push_back(ev);
    return true;
}

bool EventPump::pop(Event& out) {
    std::lock_guard<std::mutex> lock(mu_);
    if (q_.empty()) return false;
    out = q_.front();
    q_.pop_front();
    return true;
}

void EventPump::clear() {
    std::lock_guard<std::mutex> lock(mu_);
    q_.clear();
}

std::size_t EventPump::size() const {
    std::lock_guard<std::mutex> lock(mu_);
    return q_.size();
}

}  // namespace badge::core
