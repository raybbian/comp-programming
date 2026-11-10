#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"
#include "algo/common.h"

/* #include */
#include "algo/ds/segtree.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
using ds::segtree;

using mint = math::static_modint<998244353>;

// f = {a, b} is x -> a * x + b; op(f, g) applies f first.
struct affine_composite {
    using Value = pair<mint, mint>;
    static Value op(Value f, Value g) {
        return {f.first * g.first, g.first * f.second + g.second};
    }
    static Value e() {
        return {1, 0};
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<pair<mint, mint>> a(n);
    for (int i = 0; i < n; i++)
        cin >> a[i].first >> a[i].second;
    segtree<affine_composite> st(a);
    for (int i = 0; i < q; i++) {
        int typ;
        cin >> typ;
        if (typ == 0) {
            int p;
            mint c, d;
            cin >> p >> c >> d;
            st.set(p, {c, d});
        } else {
            int l, r;
            mint x;
            cin >> l >> r >> x;
            auto [c, d] = st.query(l, r - 1);
            cout << c * x + d << '\n';
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
