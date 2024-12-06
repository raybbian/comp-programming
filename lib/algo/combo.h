#pragma once
#include <algo/common.h>

#include <algo/modint.h>

template <int MAXN, int MOD> struct Combo {
    using mint = ModInt<MOD>;
    mint fact[MAXN];
    mint inv[MAXN];

    Combo() {
        fact[0] = 1;
        for (int i = 1; i < MAXN; i++)
            fact[i] = fact[i - 1] * i;
        inv[MAXN - 1] = mint(1) / fact[MAXN - 1];
        for (int i = MAXN - 2; i >= 0; i--)
            inv[i] = inv[i + 1] * (i + 1);
    }

    mint c(int n, int k) { return fact[n] * inv[k] * inv[n - k]; }
    mint p(int n, int k) { return fact[n] * inv[n - k]; }
};
