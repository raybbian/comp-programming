#pragma once
#include "../common.h"
#include "../math/common.h"

namespace algo::math {

template <typename T>
T fact(int n) {
    static std::vector<T> F(MAXN);
    static bool init = false;
    if (!init) {
        F[0] = T(1);
        for (int i = 1; i < MAXN; i++) {
            F[i] = F[i - 1] * T(i);
        }
        init = true;
    }
    return F[n];
}
template <typename T>
T inv_fact(int n) {
    static std::vector<T> F(MAXN);
    static bool init = false;
    if (!init) {
        int t = std::min(T::mod(), MAXN) - 1;
        F[t] = T(1) / fact<T>(t);
        for (int i = t - 1; i >= 0; i--) {
            F[i] = F[i + 1] * T(i + 1);
        }
        init = true;
    }
    return F[n];
}
template <typename T>
T cmb(int n, int r) {
    assert(n < MAXN);
    if (r < 0 || r > n) {
        return T(0);
    } else {
        return fact<T>(n) * inv_fact<T>(r) * inv_fact<T>(n - r);
    }
}
template <typename T>
T perm(int n, int r) {
    assert(n < MAXN);
    if (r < 0 || r > n) {
        return T(0);
    } else {
        return fact<T>(n) * inv_fact<T>(n - r);
    }
}

} // namespace algo::math
