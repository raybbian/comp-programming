#pragma once
#include "algo/common.h"

namespace algo::hash {

// Pair hash + avoid getting hacked
struct chash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
    template <typename T, typename U>
    size_t operator()(const std::pair<T, U> &x) const {
        uint64_t a = (*this)(x.fi);
        uint64_t b = (*this)(x.se);
        return a + 3 * b;
    }
};

} // namespace algo::hash
