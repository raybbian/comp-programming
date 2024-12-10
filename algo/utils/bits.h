#pragma once
#include "algo/common.h"

namespace algo::utils {

// Returns number of set bits in x
constexpr int popcnt(int64_t x) {
    return __builtin_popcountll(x);
}
// Returns floor(log_2(x))
constexpr int lg2(uint64_t x) {
    return std::bit_width(x) - 1;
}

} // namespace algo::utils
