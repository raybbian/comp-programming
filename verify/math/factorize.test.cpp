#define PROBLEM "https://judge.yosupo.jp/problem/factorize"
#include "algo/common.h"

/* #include */
#include "algo/math/pollard_rho.h"

using namespace std;
using namespace algo;
using namespace math;

void solve() {
    uint64_t a;
    cin >> a;
    auto f = factorize(a);
    cout << f.size();
    for (auto p : f)
        cout << ' ' << p;
    cout << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
        solve();
}
