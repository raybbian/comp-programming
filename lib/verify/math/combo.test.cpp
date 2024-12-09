// competitive-verifier: PROBLEM https://judge.yosupo.jp/problem/binomial_coefficient_prime_mod
#define ALGO_MAXN 1e7
#include "algo/common.h"

#include "algo/utils/bits.h"
#include "algo/utils/ndvec.h"

#ifdef LOCAL
#include "algo/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__) noop()
#else
#define dbg(...) 42
#endif

/* #include */
#include "algo/math/combo.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
#define int long long

void solve() {
    int n, k;
    cin >> n >> k;
    cout << math::cmb<math::mint>(n, k) << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t, m;
    cin >> t >> m;
    math::mint::set_mod(m);
    while (t--)
        solve();
}
