#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Primality test for any 64-bit n. Works at compile time.
//   is_prime(1'000'000'007)  // true
constexpr bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37})
        if (n % p == 0) return n == p;
    int s = std::countr_zero(n - 1);
    uint64_t d = (n - 1) >> s;
    for (uint64_t a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        uint64_t x = pow_mod(a, d, n);
        if (x == 0 || x == 1 || x == n - 1) continue;
        for (int i = 1; i < s && x != n - 1; i++)
            x = mul_mod(x, x, n);
        if (x != n - 1) return false;
    }
    return true;
}

} // namespace algo::math
