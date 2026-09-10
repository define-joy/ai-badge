#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace badge::core {

class JobQueue {
public:
    void post(std::function<void()> job);
    void drain();
    void clear();
    bool empty() const;

private:
    mutable std::mutex mu_;
    std::vector<std::function<void()>> jobs_;
};

}  // namespace badge::core
