#define PROBLEM "https://judge.yosupo.jp/problem/unionfind"
#include "algo/common.h"

/* #include */
#include "algo/ds/dsu.h"

using namespace std;
using namespace algo;
using ds::dsu;

void solve() {
    int n, q;
    cin >> n >> q;
    dsu dsu(n);
    for (int i = 0; i < q; i++) {
        int t, u, v;
        cin >> t >> u >> v;
        if (t == 0)
            dsu.unite(u, v);
        else
            cout << (dsu.is_same(u, v) ? 1 : 0) << '\n';
    }
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
