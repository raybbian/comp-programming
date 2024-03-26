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
#define each(i, a) for (auto &(i) : (a))

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
/**
 * Description: Performs range queries and range updates via lazy propagation. Bounds are
 * zero-indexed on [l, r]. Source:
 * https://www.hackerearth.com/practice/notes/segment-tree-and-lazy-propagation/ Verification:
 * https://www.spoj.com/problems/HORRIBLE/ Time: O(n) build, O(log n) query and update
 */

struct SegmentTree {
    int n;
    vector<pii> st, lazy;

    SegmentTree(int _n) : n(_n), st(4 * n, {0, -1}), lazy(4 * n, {0, -1}) {}

    void push(int p, int l, int r) {
        if (lazy[p] != pii{0, -1}) {
            st[p] = max(st[p], lazy[p]);
            if (l != r) {
                lazy[2 * p] = lazy[p];
                lazy[2 * p + 1] = lazy[p];
            }
            lazy[p] = {0, -1};
        }
    }

    pii query(int p, int l, int r, int i, int j) {
        push(p, l, r);
        if (i > r || j < l)
            return {0, -1};
        if (i <= l && r <= j)
            return st[p];
        int m = (l + r) / 2;
        return max(query(2 * p, l, m, i, j), query(2 * p + 1, m + 1, r, i, j));
    }

    pii query(int i, int j) { return query(1, 0, n - 1, i, j); }

    void update(int p, int l, int r, int i, int j, pii val) {
        push(p, l, r);
        if (i > r || j < l)
            return;
        if (i <= l && r <= j) {
            st[p] = max(st[p], val);
            if (l != r) {
                lazy[2 * p] = val;
                lazy[2 * p + 1] = val;
            }
            return;
        }
        int m = (l + r) / 2;
        update(2 * p, l, m, i, j, val);
        update(2 * p + 1, m + 1, r, i, j, val);
        st[p] = max(st[2 * p], st[2 * p + 1]);
    }

    void update(int i, int j, pii val) { update(1, 0, n - 1, i, j, val); }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<pii>> segs(n + 1);
    set<int> coord;
    rep(i, m) {
        int row, l, r;
        cin >> row >> l >> r;
        segs[row].push_back({l, r});
        coord.insert(l);
        coord.insert(r);
    }
    int cnt = 0;
    map<int, int> compress;
    each(i, coord) {
        compress[i] = cnt;
        cnt++;
    }

    vector<int> prev(n + 1, -1);
    SegmentTree seg(cnt);
    rep(i, 1, n) {
        pii best_prev = {0, 0};
        each(s, segs[i]) { best_prev = max(best_prev, seg.query(compress[s.fi], compress[s.se])); }
        prev[i] = best_prev.se;
        each(s, segs[i]) { seg.update(compress[s.fi], compress[s.se], {best_prev.fi + 1, i}); }
    }

    pii best = seg.query(0, cnt - 1);
    vector<bool> used(n + 1);
    int cur = best.se;
    while (cur != -1) {
        used[cur] = true;
        cur = prev[cur];
    }

    cout << n - best.fi << ln;
    rep(i, 1, n) if (!used[i]) { cout << i << " "; }
    cout << ln;
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
