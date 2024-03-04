#include <bits/stdc++.h>
#include <iostream>
using namespace std;

// typing is hard
using ll = long long;
using db = long double;
using str = string;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using pdd = pair<db, db>;
#define fi first
#define se second
#define ln "\n"

// loops
#define REP2(i, n) for (int i = 0; i < (n); i++)
#define REP3(i, a, b) for (int i = (a); i <= (b); i++)
#define PER2(i, n) for (int i = (n)-1; i >= 0; i--)
#define PER3(i, a, b) for (int i = (a); i >= (b); i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
#define per(...) GET_MACRO(__VA_ARGS__, PER3, PER2)(__VA_ARGS__)
#define each(i, a) for (auto &i : (a))

// n-dimensional vector
template <typename T> vector<T> ndvec(size_t size, T initial_value) {
    return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
    auto inner = ndvec<T>(u...);
    return vector<decltype(inner)>(head, inner);
}

// bitwise ops
constexpr int popcnt(int x) { return __builtin_popcount(x); }            // # of bits set
constexpr int bits(int x) { return x == 0 ? 0 : 31 - __builtin_clz(x); } // floor(log2(x))

// better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

// check and assign max and min
template <typename T> bool chk_min(T &a, const T &b) { return b < a ? a = b, 1 : 0; }
template <typename T> bool chk_max(T &a, const T &b) { return a < b ? a = b, 1 : 0; }

#ifdef LOCAL
#include "/home/rayb/Documents/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

void solve() {
    int n, d;
    cin >> n >> d;
    vector<vector<int>> adj(n + 1);
    rep(i, n - 1) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int m1, m2;
    unordered_set<int> req1, req2;
    cin >> m1;
    rep(i, m1) {
        int a;
        cin >> a;
        req1.insert(a);
    }
    cin >> m2;
    rep(i, m2) {
        int a;
        cin >> a;
        req2.insert(a);
    }

    dbg(req1, req2);

    vector<int> path;
    auto dfs1 = [&](int node, int par, auto &&dfs1) -> void {
        path.push_back(node);

        int qry = max(0, (int)path.size() - 1 - d);
        if (req1.count(node)) {
            req2.insert(path[qry]);
        }
        if (req2.count(node)) {
            req1.insert(path[qry]);
        }

        each(next, adj[node]) {
            if (next != par) {
                dfs1(next, node, dfs1);
            }
        }

        path.pop_back();
    };
    dfs1(1, -1, dfs1);

    int dist = 0;
    auto dfs2 = [&](int node, int par, unordered_set<int> &need, auto &&dfs2) -> bool {
        bool found = need.count(node);

        dist++;
        dbg(node, dist);
        if (need.count(node)) {
            need.erase(node);
        }

        bool children_has = false;
        each(next, adj[node]) {
            if (next != par) {
                children_has = dfs2(next, node, need, dfs2) || children_has;
            }
        }

        if (par != -1) {
            if (!(found || children_has))
                dist--;
            else
                dist++;
        }
        return found || children_has;
    };

    dfs2(1, -1, req1, dfs2);
    dist--;
    dfs2(1, -1, req2, dfs2);
    dist--;

    cout << dist << "\n";
}

int main() {
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
