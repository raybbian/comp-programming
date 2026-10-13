#include "algo/common.h"
#include "algo/debug/preamble.h"

/* start include */
/* end include */

#include "algo/debug/debug.h"

using namespace std;
using namespace algo;

void solve() {
    int n;
    cin >> n;
    vector<int> b(n);
    for (int i = 0; i < n; i++) {
        cin >> b[i];
    }
    vector<int> l(n);
    for (int i = 0; i < n; i++) {
        l[i] = n - 1 - b[i];
    }

    vector<int> ind(n);
    iota(ind.begin(), ind.end(), 0);
    sort(ind.begin(), ind.end(), [&](int x, int y) { return b[x] > b[y]; });

    vector<vector<int>> win(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        int need = b[ind[i]];
        b[ind[i]] -= need;

        vector<int> lose(n);
        iota(lose.begin(), lose.end(), 0);
        sort(lose.begin(), lose.end(),
             [&](int x, int y) { return l[x] > l[y]; });

        for (int j = 0; j < n && need > 0; j++) {
            if (lose[j] == ind[i]) continue;
            if (win[lose[j]][ind[i]]) continue;
            if (l[lose[j]] > 0) {
                l[lose[j]]--;
                need--;
                win[ind[i]][lose[j]] = 1;
            }
        }
        if (need > 0) {
            cout << "NO\n";
            return;
        }
        dbg(b, l, win);
    }

    cout << "YES\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << win[i][j];
        }
        cout << '\n';
    }
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
