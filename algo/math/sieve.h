#pragma once
#include "../common.h"
#include "../math/common.h"

namespace algo::math {

bool is_prime_sieve(int n) {
    assert(n < MAXN);
    static std::vector<bool> F(MAXN, true);
    static bool init = false;
    if (!init) {
        F[0] = F[1] = false;
        for (int i = 2; i * i < MAXN; i++) {
            if (F[i]) {
                for (int j = i * i; j <= n; j += i) {
                    F[j] = false;
                }
            }
        }
        init = true;
    }
    return F[n];
}

} // namespace algo::math
