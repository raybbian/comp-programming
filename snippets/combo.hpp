#include "modint.hpp"

template <int MAXN> struct Combo {
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

    mint choose(int n, int k) { return fact[n] * inv[k] * inv[n - k]; }
};

Combo<250005> cmb;
