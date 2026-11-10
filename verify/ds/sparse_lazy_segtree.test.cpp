#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"
#include "algo/common.h"

/* #include */
#include "algo/ds/sparse_lazy_segtree.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
using ds::sparse_lazy_segtree;

using mint = math::static_modint<998244353>;

struct affine_sum {
    using Value = mint;
    using Update = pair<mint, mint>;
    static Value op(Value a, Value b) {
        return a + b;
    }
    static Value e() {
        return 0;
    }
    static Update id() {
        return {1, 0};
    }
    static Update composition(Update f, Update g) {
        return {f.first * g.first, f.first * g.second + f.second};
    }
    static Value mapping(Update f, Value x, int len) {
        return f.first * x + f.second * len;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    sparse_lazy_segtree<affine_sum> st(0, n - 1);
    for (int i = 0; i < n; i++) {
        mint a;
        cin >> a;
        st.set(i, a);
    }
    for (int i = 0; i < q; i++) {
        int typ, l, r;
        cin >> typ >> l >> r;
        if (typ == 0) {
            int b, c;
            cin >> b >> c;
            st.apply(l, r - 1, {b, c});
        } else {
            cout << st.query(l, r - 1) << '\n';
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
