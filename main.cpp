#include "algo/common.h"
#include "algo/debug/preamble.h"

/* start include */
#include "algo/ds/dsu.h"
#include "algo/ds/fenwick.h"
#include "algo/ds/sparse_table.h"
#include "algo/math/modint.h"
/* end include */

#include "algo/debug/debug.h"

using namespace std;
using namespace algo;

void solve() {
    vector<bool> a = {1, 0, 0, 1, 1, 1, 0, 1};
    math::mint::set_mod(1000000007);
    math::mint b = 321931720389;
    vector<math::mint> c = {102380918203, 321378012937, 31231231208,
                            3213010101};
    string d = "TNASRIEOTNRIAS";
    ds::dsu dsu(10);

    dsu.unite(0, 1);
    dsu.unite(0, 2);
    dsu.unite(0, 3);
    dsu.unite(0, 4);
    dsu.unite(5, 6);

    bitset<10> bs(2138);

    ds::fenwick<math::mint> t({5, 1, 3, 2, 6});
    t.add(2, 10);

    ds::sparse_table<int> st({5, 1, 2, 3, 4, 6},
                             [&](int a, int b) { return std::min(a, b); });
    tuple lots{t, st, dsu, a, c};
    vector<pair<int, int>> dirs = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

    array<int, 5> arr{0, 1, 2, 3, -1};
    priority_queue<int> pq(arr.begin(), arr.end());
    queue<int> q(arr.begin(), arr.end());

    std::complex<float> cpx{1.0, 2.0};
    dbg(a, b, c, "hi", d, dsu, t, st, st.query(1, 3), bs, lots, dirs, arr, q,
        pq, cpx);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
