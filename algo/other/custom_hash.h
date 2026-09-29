#pragma once
#include "algo/common.h"

namespace algo::hash {

template <typename T>
concept tuple_like = requires { std::tuple_size<T>::value; };

// Anti-hack hash for ints, anything std::hash takes, and pair/tuple/array
// of those.
//   std::unordered_map<std::pair<int, int>, int, chash> m;
struct chash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    static uint64_t combine(uint64_t seed, uint64_t h) {
        return seed ^ (h + 0x9e3779b97f4a7c15 + (seed << 6) + (seed >> 2));
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM =
            std::chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
    template <tuple_like T>
    size_t operator()(const T &t) const {
        return std::apply(
            [this](const auto &...xs) {
                uint64_t seed = 0;
                ((seed = combine(seed, (*this)(xs))), ...);
                return seed;
            },
            t);
    }
    // Anything else std::hash takes, e.g. strings
    template <typename T>
        requires(!tuple_like<T> && !std::is_integral_v<T>)
    size_t operator()(const T &x) const {
        return (*this)(std::hash<T>{}(x));
    }
};

} // namespace algo::hash
