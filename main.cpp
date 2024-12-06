#include <algo/common.h>
#include <algo/custom_hash.h>
#include <algo/ds/ndvec.h>
#include <algo/modint.h>
#include <algo/utils/bits.h>
#include <algo/utils/math.h>

#ifdef LOCAL
#include <algo/debug.h>
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

using mint = modint107;

void solve() {
    vi a = {1, 3, 2, 4, 5, 1, 2};
    unordered_set<pii, chash> b = {{0, 1}, {5, 3}, {2, 4}, {6, 9}};
    mint c = 11083708;
    dbg(a, b, c.v);
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}

/*
 * SANITY CHECKLIST:
 * 1. re-read the problem
 * 2. consider edge cases (n=1, overflow)
 * 3. come up with counter cases
 * 4. move on to the next problem
 * 5. re-read the problem
 * 6. re-read the problem upside-down
 * 7. re-read the problem backwords
 * 8. fucking re-read the problem
 */
