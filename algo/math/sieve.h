#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Prime lookup table. Rebuilt at least twice as large when n is out of range.
//   sieve s(1e6);
//   s.is_prime(n);
struct sieve {
    explicit sieve(int n = 0) {
        if (n > 0) is_prime(n);
    }

    bool is_prime(int n) {
        if (n >= (int)f.size()) {
            int m = std::max({n + 1, 2 * (int)f.size(), 2});
            f.assign(m, true);
            f[0] = f[1] = false;
            for (int i = 2; i <= (m - 1) / i; i++) {
                if (f[i]) {
                    for (int j = i * i; j < m; j += i) {
                        f[j] = false;
                    }
                }
            }
        }
        return f[n];
    }

private:
    std::vector<bool> f;
};

} // namespace algo::math
