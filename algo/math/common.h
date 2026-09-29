#pragma once
#include "algo/common.h"

namespace algo::math {

// x mod m, in [0, m) even for negative x
constexpr int64_t safe_mod(int64_t x, int64_t m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// Fast x % m for an m only known at run time.
//   barrett b(m);
//   b.reduce(x);  // x % m
struct barrett {
    constexpr explicit barrett(uint64_t _m) : m(_m), im(-1ULL / _m) {
        assert(1 <= _m);
    }
    uint64_t mod() const {
        return m;
    };
    uint64_t reduce(uint64_t a) const {
        uint64_t q = (uint64_t)((__uint128_t(im) * a) >> 64);
        uint64_t r = a - q * m;
        return r - (r >= m) * m;
    }

private:
    uint64_t m, im;
};

// Rounded-up and rounded-down a / b. Correct for negatives.
constexpr int64_t c_div(int64_t a, int64_t b) {
    return a / b + ((a ^ b) > 0 && a % b);
}
constexpr int64_t f_div(int64_t a, int64_t b) {
    return a / b - ((a ^ b) < 0 && a % b);
}

// x^n by repeated squaring.
//   bpow(mint(2), n);
//   bpow(m, n, identity, mat_mul);  // any op, with its identity
constexpr auto bpow(auto const &x, auto n, auto const &one, auto op) {
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
constexpr auto bpow(auto x, auto n, auto ans) {
    return bpow(x, n, ans, std::multiplies{});
}
template <typename T>
constexpr T bpow(T const &x, auto n) {
    return bpow(x, n, T(1));
}

// a * b mod m, without overflow
constexpr uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)((__uint128_t)a * b % m);
}
// x^n mod m, without overflow
constexpr uint64_t pow_mod(uint64_t x, uint64_t n, uint64_t m) {
    return bpow(x % m, n, 1 % m,
                [m](uint64_t a, uint64_t b) { return mul_mod(a, b, m); });
}

// Returns (g, x): g = gcd(a, n), x * a = g (mod n), 0 <= x < n / g.
// If g == 1, x is the inverse of a mod n.
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
