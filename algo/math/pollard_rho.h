#pragma once
#include "algo/common.h"
#include "algo/math/primality.h"

namespace algo::math {

// Some factor of composite n other than 1 and n.
uint64_t pollard_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    auto diff = [](uint64_t a, uint64_t b) { return a > b ? a - b : b - a; };
    for (uint64_t c = 1;; c++) {
        auto f = [&](uint64_t x) {
            return (uint64_t)(((__uint128_t)x * x + c) % n);
        };
        // One gcd per M steps. If a batch overshoots to n, redo it step by step.
        const int M = 128;
        uint64_t x, y = 2, ys, g = 1, q = 1;
        for (int r = 1; g == 1; r <<= 1) {
            x = y;
            for (int i = 0; i < r; i++)
                y = f(y);
            for (int k = 0; k < r && g == 1; k += M) {
                ys = y;
                for (int i = 0; i < M && i < r - k; i++) {
                    y = f(y);
                    q = mul_mod(q, diff(x, y), n);
                }
                g = std::gcd(q, n);
            }
        }
        if (g == n) {
            do {
                ys = f(ys);
                g = std::gcd(diff(x, ys), n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

// Prime factors of n, sorted, with repeats. Fine up to 1e18.
//   factorize(360)  // {2, 2, 2, 3, 3, 5}
std::vector<uint64_t> factorize(uint64_t n) {
    std::vector<uint64_t> res;
    auto go = [&](auto &self, uint64_t m) -> void {
        if (m == 1) return;
        if (is_prime(m)) {
            res.push_back(m);
            return;
        }
        uint64_t d = pollard_rho(m);
        self(self, d);
        self(self, m / d);
    };
    go(go, n);
    std::sort(res.begin(), res.end());
    return res;
}

} // namespace algo::math
