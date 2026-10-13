#pragma once
#include "algo/common.h"
#include "algo/utils/rng.h"

namespace algo::gen {

// Seed from argv[1] (a number or any string) if given, else from the clock.
//   init(argc, argv);
inline uint64_t init(int argc, char **argv) {
    uint64_t s = std::chrono::steady_clock::now().time_since_epoch().count();
    if (argc > 1) {
        std::string arg = argv[1];
        bool numeric = !arg.empty() &&
                       std::all_of(arg.begin(), arg.end(), [](unsigned char c) {
                           return std::isdigit(c);
                       });
        s = numeric ? std::stoull(arg) : std::hash<std::string>{}(arg);
    }
    random::seed(s);
    return s;
}

// Uniform integer in [lo, hi].
//   num(1, 6);  // 4
template <std::integral T, std::integral U>
std::common_type_t<T, U> num(T lo, U hi) {
    using R = std::common_type_t<T, U>;
    using D = std::conditional_t<std::is_unsigned_v<R>, uint64_t, int64_t>;
    assert(D(lo) <= D(hi));
    return R(std::uniform_int_distribution<D>(lo, hi)(random::engine()));
}

// Integer in [lo, hi] skewed towards hi (bias > 0) or lo (bias < 0): the max
// (or min) of |bias| + 1 uniform draws.
//   num(1, 100, 3);   // 97
//   num(1, 100, -3);  // 4
template <std::integral T, std::integral U>
std::common_type_t<T, U> num(T lo, U hi, int bias) {
    auto x = num(lo, hi);
    for (int i = 0; i < std::abs(bias); i++) {
        auto y = num(lo, hi);
        x = bias > 0 ? std::max(x, y) : std::min(x, y);
    }
    return x;
}

// Uniform real in [lo, hi).
//   real(0, 1);  // 0.5772
inline double real(double lo, double hi) {
    return std::uniform_real_distribution<double>(lo, hi)(random::engine());
}

// True with probability p.
//   chance(0.1);  // false
inline bool chance(double p) {
    return real(0, 1) < p;
}

// Uniformly random element.
//   pick(v);
//   pick({2, 3, 5, 7});  // 5
template <std::ranges::sized_range T>
auto pick(const T &c) {
    assert(!std::ranges::empty(c));
    return *std::next(std::ranges::begin(c),
                      num(size_t(0), std::ranges::size(c) - 1));
}
template <typename T>
T pick(std::initializer_list<T> c) {
    return pick<std::initializer_list<T>>(c);
}

// Element with probability proportional to its weight.
//   pick(v, w);
//   pick({'a', 'b'}, {9, 1});  // 'a'
template <std::ranges::sized_range T, std::ranges::range W>
auto pick(const T &c, const W &w) {
    assert(std::ranges::size(c) == std::ranges::size(w));
    std::discrete_distribution<size_t> d(std::ranges::begin(w),
                                         std::ranges::end(w));
    return *std::next(std::ranges::begin(c), d(random::engine()));
}
template <typename T>
T pick(std::initializer_list<T> c, std::initializer_list<double> w) {
    return pick<std::initializer_list<T>, std::initializer_list<double>>(c, w);
}

// Shuffle in place.
//   shuffle(v);
template <std::ranges::random_access_range T>
void shuffle(T &c) {
    std::shuffle(std::ranges::begin(c), std::ranges::end(c), random::engine());
}

// Random permutation of base..base+n-1.
//   perm(5);     // [3, 0, 4, 1, 2]
//   perm(5, 1);  // [2, 5, 1, 3, 4]
inline std::vector<int> perm(int n, int base = 0) {
    std::vector<int> p(n);
    std::iota(p.begin(), p.end(), base);
    shuffle(p);
    return p;
}

} // namespace algo::gen
