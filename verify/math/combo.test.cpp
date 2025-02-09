#define PROBLEM "https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod"
#define ALGO_MAXN 1e7
#include "algo/common.h"

/* #include */
#include "algo/math/combo.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
using namespace math;

void solve() {
    int n, k;
    cin >> n >> k;
    cout << cmb<mint>(n, k) << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t, m;
    cin >> t >> m;
    mint::set_mod(m);
    while (t--)
        solve();
}
