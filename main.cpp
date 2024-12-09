#include "algo/common.h"

#include "algo/math/common.h"
#include "algo/utils/bits.h"
#include "algo/utils/ndvec.h"

#ifdef LOCAL
#include "algo/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__) noop()
#else
#define dbg(...) 42
#endif

// #include
#include "algo/math/modint.h"

using namespace std;
using namespace algo;
#define int long long

void solve() {
    vector<bool> a = {1, 0, 0, 1, 1, 1, 0, 1};
    // math::mint::set_mod(1000000007);
    math::mint b = 321931720389;
    dbg(a, b);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
