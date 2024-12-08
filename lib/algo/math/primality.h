#pragma once
#include <algo/common.h>
#include <algo/math/modint.h>

/*
 * Copied from
 * https://github.com/cp-algorithms/cp-algorithms-aux/blob/main/cp-algo/number_theory/modint.hpp
 */

namespace algo::math {

// https://en.wikipedia.org/wiki/Miller–Rabin_primality_test
bool is_prime(uint64_t m) {
    if (m == 1 || m % 2 == 0) {
        return m == 2;
    }
    // m - 1 = 2^s * d
    int s = countr_zero(m - 1);
    auto d = (m - 1) >> s;
    using base = dynamic_modint<>;
    auto test = [&](base x) {
        x = bpow(x, d);
        if (abs(x.rem()) <= 1) {
            return true;
        }
        for (int i = 1; i < s && x != -1; i++) {
            x *= x;
        }
        return x == -1;
    };
    return base::with_mod(m, [&]() {
        // Works for all m < 2^64: https://miller-rabin.appspot.com
        return ranges::all_of(
            array{2, 325, 9375, 28178, 450775, 9780504, 1795265022}, test);
    });
}

} // namespace algo::math
