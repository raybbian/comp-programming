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
    vector<int> a(n + 1);
    ll sum_a = 0;
    rep(i, 1, n) {
        cin >> a[i];
        sum_a += a[i];
    }

    vector<vector<int>> adj(n + 1);
    rep(i, n - 1) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    vector<pll> dp1(n + 1);
    auto calc1 = [&](this auto const &calc1, int v, int p) -> void {
        dp1[v] = {0, a[v]};
        each(next, adj[v]) {
            if (next == p) {
                continue;
            }
            calc1(next, v);
            dp1[v].first += dp1[next].first + dp1[next].second;
            dp1[v].second += dp1[next].second;
        }
    };

    vector<ll> dp2(n + 1);
    auto calc2 = [&](this auto const &calc2, int v, int p) -> void {
        // push to next
        each(next, adj[v]) {
            if (next == p) {
                continue;
            }
            dp2[next] = dp2[v] + sum_a - 2 * dp1[next].second;
            calc2(next, v);
        }
    };

    calc1(1, 0);
    dp2[1] = dp1[1].first;
    calc2(1, 0);

    dbg(dp1, dp2);
    cout << *max_element(dp2.begin(), dp2.end()) << '\n';
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
