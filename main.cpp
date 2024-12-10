#include "algo/common.h"

#ifdef LOCAL
#include "algo/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__) noop()
#else
#define dbg(...) 42
#endif

// #include
#include "algo/ds/dsu.h"
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
#define int long long

void solve() {
    vector<bool> a = {1, 0, 0, 1, 1, 1, 0, 1};
    math::mint::set_mod(1000000007);
    math::mint b = 321931720389;
    vector<math::mint> c = {102380918203, 321378012937, 31231231208,
                            3213010101};
    string d = "TNASRIEOTNRIAS";
    algo::ds::dsu dsu(10);

    dsu.unite(0, 1);
    dsu.unite(0, 2);
    dsu.unite(0, 3);
    dsu.unite(0, 4);
    dsu.unite(5, 6);

    dbg(a, b, c, "hi", d, dsu);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
