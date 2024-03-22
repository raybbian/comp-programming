#include <bits/stdc++.h>
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
    int n, m;
    cin >> n >> m;
    vector<vector<array<int, 2>>> adj(n + 1);
    vector<array<int, 2>> edges(m);
    rep(i, m) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
        edges[i] = {u, v};
    }

    vector<int> v(n + 1, -1), in(n + 1, -1);
    vector<int> ans(m);
    auto dfs = [&](int node, int dist, auto &&dfs) -> void {
        each(next, adj[node]) {
            if (v[next[0]] == -1) {
                v[next[0]] = dist;
                ans[next[1]] = 1;
                in[next[0]] = next[1];
                dfs(next[0], dist + 1, dfs);
            }
        }
    };
    v[1] = 0;
    dfs(1, 0, dfs);

    vector<int> check;
    rep(i, m) {
        if (!ans[i]) {
            check.push_back(edges[i][0]);
            check.push_back(edges[i][1]);
        }
    }
    sort(check.begin(), check.end(), [&](int a, int b) { return v[a] > v[b]; });
    check.erase(unique(check.begin(), check.end()), check.end());

    dbg(check);
    if (m - n + 1 == 3 && (int)check.size() == 3) {
        int edge = -1;
        each(next, adj[check[0]]) {
            if (!ans[next[1]]) {
                edge = next[1];
                break;
            }
        }
        assert(edge != -1);
        swap(ans[edge], ans[in[check[0]]]);
    }

    rep(i, m) { cout << ans[i]; }
    cout << "\n";
}

int main() {
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
