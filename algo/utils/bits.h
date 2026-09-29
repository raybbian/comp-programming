#pragma once
#include "algo/common.h"

namespace algo::utils {

// Number of 1 bits in x
constexpr int popcnt(int64_t x) {
    return __builtin_popcountll(x);
}
// floor(log2(x)); -1 for x = 0
constexpr int lg2(uint64_t x) {
    return std::bit_width(x) - 1;
}

} // namespace algo::utils
