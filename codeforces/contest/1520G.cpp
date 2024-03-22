#include <bits/stdc++.h>
#include <queue>
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

const vector<pii> steps = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

void solve() {
    ll n, m, w;
    cin >> n >> m >> w;
    auto grid = ndvec<ll>(n + 1, m + 1, 0LL);
    rep(i, 1, n) rep(j, 1, m) cin >> grid[i][j];

    vector<ll> home, school;
    ll ans = 1e18;

    auto work = [&n, &m, &w, &grid, &ans](int str, int stc, vector<ll> &store) {
        using T = tuple<ll, int, int>;
        priority_queue<T, vector<T>, greater<T>> pq;
        auto dist = ndvec<ll>(n + 1, m + 1, LLONG_MAX); // LLONG_MAX OK FOR DIJK
        pq.push({0, str, stc});
        dist[str][stc] = 0;
        while (!pq.empty()) {
            dbg(pq);
            const auto [cost, r, c] = pq.top(); // DO NOT USE REFERENCE HERE!!!
            pq.pop();

            if (grid[r][c] != 0 && grid[r][c] != -1)
                store.push_back(cost + grid[r][c]);

            if (r == 1 && c == 1 && str == n && stc == m)
                ans = min(ans, cost);
            if (r == n && c == m && str == 1 && stc == 1)
                ans = min(ans, cost);

            if (dist[r][c] != cost)
                continue;

            for (const auto &[dr, dc] : steps) {
                if (r + dr < 1 || r + dr > n || c + dc < 1 || c + dc > m)
                    continue;
                if (grid[r + dr][c + dc] == -1)
                    continue;
                if (cost + w < dist[r + dr][c + dc]) {
                    dbg(r + dr, c + dc);
                    pq.push({dist[r + dr][c + dc] = cost + w, r + dr, c + dc});
                }
            }
        }
        dbg(dist);
    };

    work(1, 1, home);
    work(n, m, school);

    if (home.size() && school.size()) {
        sort(home.begin(), home.end());
        sort(school.begin(), school.end());
        ans = min(ans, *home.begin() + *school.begin());
    }

    if (ans == 1e18) {
        cout << -1 << "\n";
    } else {
        cout << ans << "\n";
    }
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
