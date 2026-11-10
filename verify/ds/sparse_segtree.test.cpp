#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include "algo/common.h"

/* #include */
#include "algo/ds/sparse_segtree.h"

using namespace std;
using namespace algo;
using ds::sparse_segtree;

void solve() {
    int n, q;
    cin >> n >> q;
    sparse_segtree<ds::sum_monoid<int64_t>> t(0, n - 1);
    for (int i = 0; i < n; i++) {
        int a;
        cin >> a;
        t.set(i, a);
    }
    for (int i = 0; i < q; i++) {
        int typ, x, y;
        cin >> typ >> x >> y;
        if (typ == 0) {
            t.set(x, t.get(x) + y);
        } else {
            cout << t.query(x, y - 1) << '\n';
        }
    }
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
