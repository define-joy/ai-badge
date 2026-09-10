#include <core/JobQueue.hpp>
#include <utility>

namespace badge::core {

void JobQueue::post(std::function<void()> job) {
    if (!job) return;
    std::lock_guard<std::mutex> lock(mu_);
    jobs_.push_back(std::move(job));
}

void JobQueue::drain() {
    std::vector<std::function<void()>> batch;
    {
        std::lock_guard<std::mutex> lock(mu_);
        batch.swap(jobs_);
    }
    for (auto& job : batch) {
        if (job) job();
    }
}

void JobQueue::clear() {
    std::lock_guard<std::mutex> lock(mu_);
    jobs_.clear();
}

bool JobQueue::empty() const {
    std::lock_guard<std::mutex> lock(mu_);
    return jobs_.empty();
}

}  // namespace badge::core
