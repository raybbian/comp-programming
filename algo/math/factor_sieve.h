#pragma once
#include "algo/common.h"

namespace algo::math {

// Smallest prime factor of each of 1..n, in O(n). Factors any x <= n quickly.
//   factor_sieve fs(1e6);
//   fs.factorize(360);  // {2, 2, 2, 3, 3, 5}
//   fs.is_prime(97);    // true
//   fs.rad(360);        // 30 = 2 * 3 * 5, product of distinct primes
//   fs.primes;          // all primes <= n
struct factor_sieve {
    explicit factor_sieve(int n) : sp(n + 1), rd(n + 1, 1) {
        for (int i = 2; i <= n; i++) {
            if (sp[i] == 0) sp[i] = rd[i] = i, primes.push_back(i);
            for (int p : primes) {
                if (p > sp[i] || (int64_t)i * p > n) break;
                sp[i * p] = p;
                rd[i * p] = p == sp[i] ? rd[i] : rd[i] * p;
            }
        }
    }
    int spf(int x) const {
        assert(2 <= x && x < (int)sp.size());
        return sp[x];
    }
    bool is_prime(int x) const {
        assert(0 <= x && x < (int)sp.size());
        return x >= 2 && sp[x] == x;
    }
    // Product of the distinct primes of x. Equal for x and y exactly when they
    // have the same prime factors.
    int rad(int x) const {
        assert(1 <= x && x < (int)sp.size());
        return rd[x];
    }
    // Prime factors of x, sorted, with repeats
    std::vector<int> factorize(int x) const {
        assert(1 <= x && x < (int)sp.size());
        std::vector<int> res;
        for (; x > 1; x /= sp[x])
            res.push_back(sp[x]);
        return res;
    }

    std::vector<int> primes;

private:
    std::vector<int> sp, rd;
};

} // namespace algo::math
