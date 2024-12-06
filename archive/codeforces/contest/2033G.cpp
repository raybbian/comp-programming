#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

constexpr int popcnt(ll x) { return __builtin_popcountll(x); }
constexpr int bits(ll x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); } // floor(log_2(x))

constexpr long long c_div(long long a, long long b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr long long f_div(long long a, long long b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    rep(i, n - 1) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    auto par = ndvec<int>(n + 1, 32, 0);
    vector<int> dp(n + 1);
    vector<int> dist(n + 1);

    auto dfs = [&](this auto const &dfs, int v, int p) -> void {
        par[v][0] = p;
        rep(i, 1, 31) { par[v][i] = par[par[v][i - 1]][i - 1]; }
        each(next, adj[v]) {
            if (next == p) {
                continue;
            }
            dfs(next, v);
            dp[v] = max(dp[v], dp[next] + 1);
        }
    };
    dfs(1, 0);

    auto sol = ndvec<ll>(n + 1, 32, 0LL);
    auto calc = [&](this auto const &calc, int v, int p, int d) -> void {
        dist[v] = d;
        rep(i, 1, 31) {
            if (par[v][i] == 0)
                break;
            sol[v][i] = max(sol[v][i - 1], sol[par[v][i - 1]][i - 1] + (1 << (i - 1)));
        }

        int l = adj[v].size();
        vector<int> cands;
        rep(i, l) {
            if (adj[v][i] == p)
                continue;
            cands.push_back(i);
        }
        sort(cands.begin(), cands.end(),
             [&dp, &adj, &v](int a, int b) { return dp[adj[v][a]] > dp[adj[v][b]]; });

        each(i, cands) {
            int next = adj[v][i];
            sol[next][0] = 1;
            if (i == cands[0]) {
                // this is best_ind
                if ((int)cands.size() >= 2)
                    sol[next][0] = max(sol[next][0], 2LL + dp[adj[v][cands[1]]]);
            } else {
                sol[next][0] = max(sol[next][0], 2LL + dp[adj[v][cands[0]]]);
            }
            calc(next, v, d + 1);
        }
    };
    calc(1, 0, 0);

    dbg(sol);
    int q;
    cin >> q;
    rep(i, q) {
        int v, k;
        cin >> v >> k;
        k = min(k, dist[v]);
        ll res = dp[v];
        int cur = v;
        int cur_dist = 0;
        per(j, 32) {
            if (k & (1 << j)) {
                res = max(res, cur_dist + sol[cur][j]);
                cur = par[cur][j];
                cur_dist |= (1 << j);
                if (cur == 0)
                    break;
            }
        }
        cout << res << " ";
    }
    cout << "\n";
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
