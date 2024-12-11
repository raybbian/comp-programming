#define PROBLEM "https://judge.yosupo.jp/problem/lca"
#include "algo/common.h"

/* #include */
#include "algo/graph/lca.h"

using namespace std;
using namespace algo;
using graph::lca;

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> p(n);
    vector<vector<int>> adj(n);
    for (int i = 1; i < n; i++) {
        cin >> p[i];
        adj[i].push_back(p[i]);
        adj[p[i]].push_back(i);
    }
    lca lca(adj);
    for (int i = 0; i < q; i++) {
        int u, v;
        cin >> u >> v;
        cout << lca.par(u, v) << '\n';
    }
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    // int t;
    // cin >> t;
    // while (t--)
    solve();
}
