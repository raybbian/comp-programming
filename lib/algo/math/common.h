#pragma once
#include "algo/common.h"

#ifdef ALGO_MAXN
const int MAXN = ALGO_MAXN;
#else
const int MAXN = 1 << 19;
#endif

namespace algo::math {

constexpr int64_t safe_mod(int64_t x, int64_t m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Returns (x ** n) % m
constexpr int64_t pow_mod_constexpr(int64_t x, int64_t n, int m) {
    assert(0 <= n);
    assert(1 <= m);
    if (m == 1) return 0;
    uint _m = (uint)(m);
    uint64_t r = 1;
    uint64_t y = safe_mod(x, m);
    while (n) {
        if (n & 1) r = (r * y) % _m;
        y = (y * y) % _m;
        n >>= 1;
    }
    return r;
}

// Fast modular multiplication by barrett reduction
// Copied from:
// https://github.com/atcoder/ac-library/blob/master/atcoder/internal_math.hpp
struct barrett {
    uint m;
    uint64_t im;

    explicit barrett(uint _m) : m(_m), im((uint64_t)(-1) / _m + 1) {
        assert(1 <= _m);
    }

    uint umod() const {
        return m;
    }

    // Returns a * b % m
    uint mul(uint a, uint b) const {
        assert(a < m);
        assert(b < m);

        uint64_t z = a;
        z *= b;
        uint64_t x = (uint64_t)(((__uint128_t)(z)*im) >> 64);
        uint64_t y = x * m;
        return (uint)(z - y + (z < y ? m : 0));
    }
};

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
    return bpow(x, n, ans, std::multiplies{});
}
template <typename T>
T bpow(T const &x, auto n) {
    return bpow(x, n, T(1));
}

// Returns a pair(g, x) s.t. g = gcd(a, n), xa = g (mod n), 0 <= x < n/g
// If r > 1 then a is not invertible mod n
constexpr std::pair<int64_t, int64_t> inv_gcd(int64_t a, int64_t n) {
    a = safe_mod(a, n);
    if (a == 0) return {n, 0};

    int64_t t = 0, newt = 1;
    int64_t r = n, newr = a;

    while (newr) {
        int64_t quotient = r / newr;
        r -= newr * quotient;
        t -= newt * quotient;

        std::swap(r, newr);
        std::swap(t, newt);
    }
    if (t < 0) t += n / r;
    return {r, t};
}

} // namespace algo::math
