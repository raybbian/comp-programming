#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"
#include "algo/common.h"

/* #include */
#include "algo/math/combo.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
using namespace math;

using mint = dynamic_modint<>;

void solve(combo<mint> &C) {
    int n, k;
    cin >> n >> k;
    cout << C.cmb(n, k) << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t, m;
    cin >> t >> m;
    mint::with_mod(m, [&] {
        combo<mint> C;
        while (t--)
            solve(C);
    });
}
