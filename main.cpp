#ifndef PREPROCESS
#include <bits/stdc++.h>
#endif
using namespace std;

#ifdef LOCAL
#include "lib/debug/debug.h"
#define dbg(...) FOR_EACH(DEBUG, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

#include "lib/utils/loops.h"
#include "lib/utils/math.h"
#include "lib/utils/ndvec.h"

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// insert snippets below:

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n), pre(n + 1);
    rep(i, n) {
        cin >> a[i];
        pre[i + 1] = a[i] + pre[i];
    }
    ll mn = LLONG_MAX, mx = LLONG_MIN;
    rep(i, 1, n) { mn = min(mn, pre[i] / i); }
    reverse(a.begin(), a.end());
    rep(i, n) pre[i + 1] = a[i] + pre[i];
    rep(i, 1, n) { mx = max(mx, c_div(pre[i], i)); }

    cout << mx - mn << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    int t;
    cin >> t;
    while (t--)
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
