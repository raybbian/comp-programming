#pragma once
#include <algo/common.h>

namespace algo::math {

constexpr int64_t c_div(int64_t a, int64_t b) {
    return a / b + ((a ^ b) > 0 && a % b);
}
constexpr int64_t f_div(int64_t a, int64_t b) {
    return a / b - ((a ^ b) < 0 && a % b);
}

auto bpow(auto const &x, auto n, auto const &one, auto op) {
    if (n == 0) {
        return one;
    } else {
        auto t = bpow(x, n / 2, one, op);
        t = op(t, t);
        if (n % 2) {
            t = op(t, x);
        }
        return t;
    }
}
auto bpow(auto x, auto n, auto ans) {
    return bpow(x, n, ans, multiplies{});
}
template <typename T>
T bpow(T const &x, auto n) {
    return bpow(x, n, T(1));
}

} // namespace algo::math
