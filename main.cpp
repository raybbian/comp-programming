#include "algo/common.h"
#include "algo/debug/preamble.h"

/* start include */
#include "algo/math/combo.h"
#include "algo/math/modint.h"
/* end include */

#include "algo/debug/debug.h"

using namespace std;
using namespace algo;

using mint = math::static_modint<998244353>;
math::combo<mint> C;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n - 1);
    for (int i = 0; i < n - 1; i++) {
        cin >> a[i];
    }
    // for s_i = a_i, then we must have that s_i is max on one side or the other
    // if we say that it is the max on the side that gets smaller as we iterate
    // then, we must have the max always decreases
    // some LIS/increasing subsequence thing
    // max on other side is awlays N
    // aggregate some number of Ns on the ends
    // if N position is fixed at end, then s_i must be increasing at (they must
    // all be start half)
    // in that case, the number of such permutations is every time that it
    // increases, then p_i = s_i
    // (choose s_i - num_increases, num_consecutive - 1)
    // we can treat s_i as max so far, or max on other side

    // num permutations that match s_i to index i, both directions

    // we have every distinct number up to peak is fixed.
    // for X spots, we must have <= K
    // for Y spots, we must have <= J

    // 3 1 4 5 2
    // 3 1 5 4 2
    // highest bucket: some permutation of numbers must happen
    // 3 _ 6 _ _ 5
    // 3 _ 5 _ _ 6
    // we choose from smallest to largest
    // doesn't matter what we choose, because the other is eligible anyways
    // 3 -> 2 less, (2, 1)
    // 5 -> 4-2 less -> (3, 2)
    // a is prefix maxes left of n, then suffix maxes right of n, so it has to
    // rise then fall
    int ptr = 0;
    while (ptr + 1 < n - 1 && a[ptr] <= a[ptr + 1]) {
        ptr++;
    }
    while (ptr + 1 < n - 1 && a[ptr] >= a[ptr + 1]) {
        ptr++;
    }
    if (ptr != n - 2) {
        dbg(a, "bad, not peak");
        cout << 0 << '\n';
        return;
    }
    vector<int> fst(n + 1, -1), lst(n + 1, -1);
    int last_seen = -1;
    for (int i = 0; i < n - 1; i++) {
        if (last_seen != a[i] && fst[a[i]] != -1) {
            // if last_seen is not itself, but we've seen before, then we bad
            dbg(i, a, "bad, dup num");
            cout << 0 << '\n';
            return;
        }
        if (last_seen != a[i]) {
            fst[a[i]] = i;
        }
        lst[a[i]] = i;
        last_seen = a[i];
    }
    int mx_el = *max_element(a.begin(), a.end());
    if (mx_el != n - 1) {
        dbg("bad max val");
        cout << 0 << '\n';
        return;
    }
    mint ans = 1;
    int num_used = 0;
    for (int el = 1; el <= mx_el; el++) {
        if (fst[el] == -1) {
            continue;
        }
        int len = lst[el] - fst[el];
        int num_avail = el - num_used - 1;
        if (num_avail < len) {
            dbg(el, num_avail, len, "bad, not enough num");
            cout << 0 << '\n';
            return;
        }
        ans *= C.perm(num_avail, len);
        if (el == mx_el) {
            // one edge must be highest value
            ans *= 2;
        }
        dbg(el, fst[el], lst[el], num_avail, ans);
        // we used len + 1 numbers
        num_used += len + 1;
    }
    cout << ans << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
        solve();
}
