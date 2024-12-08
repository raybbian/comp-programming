#include <algo/common.h>

#include <algo/math/factorize.h>
#include <algo/math/modint.h>

using algo::math::factorize;

#ifdef LOCAL
#include <algo/debug.h>
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

using namespace std;
using ll = long long;
// #define int long long

void solve() {
    vector<ll> a = {1, 23, 4, 2, 3};
    vector<bool> b = {1, 0, 0, 1, 1, 1, 0};
    bitset<64> c(3218);

    vector<ll> factors;
    for (ll i : factorize(123109332348830921)) {
        factors.push_back(i);
    }

    dbg(a, b, c, factors);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
