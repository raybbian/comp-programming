#pragma once
#include "algo/common.h"

namespace algo::random {

// Shared 64-bit engine, seeded by time unless seed() is called.
inline std::mt19937_64 &engine() {
    static std::mt19937_64 e(
        std::chrono::steady_clock::now().time_since_epoch().count());
    return e;
}
inline void seed(uint64_t s) {
    engine().seed(s);
}

// Random 64-bit number
inline uint64_t rng() {
    return engine()();
}

} // namespace algo::random
