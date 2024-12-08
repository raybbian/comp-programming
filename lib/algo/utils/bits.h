#pragma once
#include <algo/common.h>

namespace algo::utils {

// Returns number of set bits in x
constexpr int popcnt(int64_t x) {
    return __builtin_popcountll(x);
}
// Returns floor(log_2(x))
constexpr int lg2(int64_t x) {
    return x == 0 ? 0 : 63 - __builtin_clzll(x);
}

} // namespace algo::utils
