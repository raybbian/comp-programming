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
struct DSU {
    vector<int> e;
    DSU(int n) { e = vector<int>(n, -1); }

    int get(int x) { return e[x] < 0 ? x : e[x] = get(e[x]); }

    bool is_same(int a, int b) { return get(a) == get(b); }

    int size(int x) { return -e[get(x)]; }

    bool unite(int x, int y) {
        x = get(x), y = get(y);
        if (x == y)
            return false;
        if (e[x] > e[y])
            swap(x, y);
        e[x] += e[y];
        e[y] = x;
        return true;
    }
};

void solve() {
    int n;
    cin >> n;
    vector<array<ll, 4>> pts;
    vector<array<ll, 2>> interval(n);
    rep(i, n) {
        ll c, l, r;
        cin >> c >> l >> r;
        interval[i] = {l, r};
        pts.push_back({l, -1, c, i});
        pts.push_back({r, 1, c, i});
    }

    sort(pts.begin(), pts.end());

    DSU dsu(n);
    set<ll> red, blue, ans;
    each(info, pts) {
        const auto [pt, t, c, i] = info;
        if (t == -1) {
            if (c == 0) {
                red.insert(i);
                each(other, blue) { dsu.unite(i, other); }
                auto best = max_element(blue.begin(), blue.end(), [&](ll a, ll b) {
                    return interval[a][1] < interval[b][1];
                });
                if (best != blue.end()) {
                    blue.clear();
                    blue.insert(*best);
                }
            } else {
                blue.insert(i);
                each(other, red) { dsu.unite(i, other); }
                auto best = max_element(red.begin(), red.end(), [&](ll a, ll b) {
                    return interval[a][1] < interval[b][1];
                });
                if (best != red.end()) {
                    red.clear();
                    red.insert(*best);
                }
            }
        } else {
            if (c == 0) {
                red.erase(i);
            } else {
                blue.erase(i);
            }
        }
        dbg(red, blue);
    }

    rep(i, n) { ans.insert(dsu.get(i)); }
    cout << (int)ans.size() << ln;
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
