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
    int q, a0, c0;
    cin >> q >> a0 >> c0;
    vector<array<ll, 2>> cost(q + 1);
    vector<int> height(q + 1);
    int msb = bits(q);
    vector<vector<int>> up(q + 1, vector<int>(msb + 1));

    cost[0] = {a0, c0};
    up[0][0] = 0;
    height[0] = 0;

    rep(i, 1, q) {
        int type;
        cin >> type;
        if (type == 1) {
            int p, a, c;
            cin >> p >> a >> c;
            cost[i] = {a, c};
            up[i][0] = p;
            height[i] = height[p] + 1;
            rep(j, 1, msb) { up[i][j] = up[up[i][j - 1]][j - 1]; }
        } else {
            int v;
            ll w;
            cin >> v >> w;
            ll _w = w;
            int ori_v = v;

            per(j, msb, 0) {
                // if not 0, go up
                if (cost[up[v][j]][0] > 0) {
                    v = up[v][j];
                }
            }

            dbg(v);
            ll money = 0;
            while (w > 0) {
                ll amt = min(w, cost[v][0]);
                if (amt == 0)
                    break;
                w -= amt;
                cost[v][0] -= amt;
                money += amt * cost[v][1];

                dbg(v);

                // move v = next v down
                int _v = ori_v;
                per(j, msb, 0) {
                    if (height[up[_v][j]] > height[v]) {
                        _v = up[_v][j];
                    }
                }
                v = _v;
                dbg(v);
            }

            cout << _w - w << " " << money << "\n";
            cout.flush();
        }
    }
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
