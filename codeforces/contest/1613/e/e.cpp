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
const vector<pii> steps = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

void solve() {
    int n, m;
    cin >> n >> m;
    auto grid = ndvec<char>(n + 1, m + 1, '.');
    pii L = {-1, -1};
    rep(i, 1, n) rep(j, 1, m) {
        cin >> grid[i][j];
        if (grid[i][j] == 'L')
            L = {i, j};
    }

    auto v = ndvec<bool>(n + 1, m + 1, false);

    auto ok = [&](int r, int c) {
        // can be forced onto visited
        int ans = 4;
        for (const auto &[dr, dc] : steps) {
            if (r + dr < 1 || r + dr > n || c + dc < 1 || c + dc > m) {
                ans--;
                continue;
            }
            if (grid[r + dr][c + dc] == '#') {
                ans--;
                continue;
            }
            if (v[r + dr][c + dc]) {
                ans--;
                continue;
            }
        }
        return ans <= 1;
    };

    queue<pii> q;
    q.push(L);
    v[L.fi][L.se] = 1;
    while (!q.empty()) {
        const auto [r, c] = q.front();
        q.pop();

        for (const auto &[dr, dc] : steps) {
            if (r + dr < 1 || r + dr > n || c + dc < 1 || c + dc > m)
                continue;
            if (grid[r + dr][c + dc] == '#')
                continue;
            if (v[r + dr][c + dc])
                continue;
            if (!ok(r + dr, c + dc)) {
                continue;
            }
            v[r + dr][c + dc] = true;
            q.push({r + dr, c + dc});
        }
    }

    dbg(v);
    rep(i, 1, n) rep(j, 1, m) {
        if (v[i][j] && grid[i][j] != 'L')
            cout << '+';
        else
            cout << grid[i][j];
        if (j == m)
            cout << "\n";
    }
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
