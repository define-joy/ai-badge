#pragma once

#include <cstdint>

namespace badge {

enum class Status : std::uint8_t {
    Ok = 0,
    NotReady,      // 外设 probe 失败或未初始化
    Busy,
    Timeout,
    InvalidArg,
    Unsupported,   // 本板没有这路能力（编译期有、运行时无，或相反）
    IoError,
};

inline bool ok(Status s) { return s == Status::Ok; }

}  // namespace badge
