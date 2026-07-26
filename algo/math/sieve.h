#pragma once
#include "algo/common.h"
#include "algo/math/common.h"

namespace algo::math {

// Rebuilt on demand, doubling the bound each time it is outgrown.
struct sieve {
    explicit sieve(index_t n = 0) {
        if (n > 0) is_prime(n);
    }

    bool is_prime(index_t n) {
        if (n >= (index_t)f.size()) {
            index_t m = std::max({n + 1, 2 * (index_t)f.size(), index_t(2)});
            f.assign(m, true);
            f[0] = f[1] = false;
            for (index_t i = 2; i <= (m - 1) / i; i++) {
                if (f[i]) {
                    for (index_t j = i * i; j < m; j += i) {
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
