#pragma once
#include <algo/common.h>

namespace algo::math {

vector<bool> is_prime;
constexpr void sieve(int n) {
    is_prime = vector<bool>(n + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

} // namespace algo::math
