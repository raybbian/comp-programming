#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"
#include "algo/common.h"

#ifdef LOCAL
#include "algo/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__) noop()
#else
#define dbg(...) 42
#endif

/* #include */
#include "algo/ds/fenwick.h"

using namespace std;
using namespace algo;
using ds::fenwick;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int64_t> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i];
    fenwick<int64_t> t(a);
    for (int i = 0; i < q; i++) {
        int typ, x, y;
        cin >> typ >> x >> y;
        if (typ == 0) {
            t.add(x, y);
        } else {
            cout << t.sum(x, y - 1) << '\n';
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
