#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"
#include "algo/common.h"

#ifdef LOCAL
#include "algo/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__) noop()
#else
#define dbg(...) 42
#endif

/* #include */
#include "algo/ds/sparse_table.h"

using namespace std;
using namespace algo;
using ds::sparse_table;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    sparse_table st(a);
    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        cout << st.query(l, r - 1) << '\n';
    }
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
