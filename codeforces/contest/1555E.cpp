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

// snippets
struct RURQ {
    struct F { // lazy update
        ll inc = 0;
        F() {}
        F(int x) { inc = x; }
        F &operator*=(const F &a) {
            inc += a.inc;
            return *this;
        }
    };
    vector<F> lazy;
    struct T { // data you need to store for each interval
        ll sz = 1, mn = 1e9, sum = 0;
        T() {}
        T(int x) { mn = sum = x; }
        friend T operator+(const T &a, const T &b) {
            T res;
            res.sz = a.sz + b.sz;
            res.mn = min(a.mn, b.mn), res.sum = a.sum + b.sum;
            return res;
        }
        T &operator*=(const F &a) {
            mn += a.inc;
            sum += (ll)sz * a.inc;
            return *this;
        }
    };
    vector<T> seg;
    int SZ = 1;
    void init(const vector<T> &_seg) { //_seg len must be power of 2!
        while (SZ < (int)_seg.size())
            SZ *= 2;
        seg.resize(2 * SZ);
        lazy.resize(2 * SZ);
        for (int i = 0; i < SZ; i++)
            seg[SZ + i] = _seg[i];
        for (int i = SZ; i >= 1; i--)
            pull(i);
    }
    void push(int ind) { /// modify values for current node
        seg[ind] *= lazy[ind];
        if (ind < SZ)
            for (int i = 0; i < 2; i++)
                lazy[2 * ind + i] *= lazy[ind];
        lazy[ind] = F();
    } // recalc values for current node
    void pull(int ind) { seg[ind] = seg[2 * ind] + seg[2 * ind + 1]; }
    void upd(int lo, int hi, F inc, int ind, int L, int R) {
        push(ind);
        if (hi < L || R < lo)
            return;
        if (lo <= L && R <= hi) {
            lazy[ind] = inc;
            push(ind);
            return;
        }
        int M = (L + R) / 2;
        upd(lo, hi, inc, 2 * ind, L, M);
        upd(lo, hi, inc, 2 * ind + 1, M + 1, R);
        pull(ind);
    }
    void upd(int lo, int hi, int inc) { upd(lo, hi, {inc}, 1, 0, SZ - 1); }
    T query(int lo, int hi, int ind, int L, int R) {
        push(ind);
        if (lo > R || L > hi)
            return T();
        if (lo <= L && R <= hi)
            return seg[ind];
        int M = (L + R) / 2;
        return query(lo, hi, 2 * ind, L, M) + query(lo, hi, 2 * ind + 1, M + 1, R);
    }
    T query(int lo, int hi) { return query(lo, hi, 1, 0, SZ - 1); }
};

void solve() {
    int n, m;
    cin >> n >> m;
    vector<array<int, 3>> interval(n);
    rep(i, n) { cin >> interval[i][0] >> interval[i][1] >> interval[i][2]; }
    sort(interval.begin(), interval.end(),
         [](array<int, 3> &a, array<int, 3> &b) { return a[2] < b[2]; });

    dbg(interval);
    RURQ seg;
    seg.init(vector<RURQ::T>((1 << 20), RURQ::T(0)));
    int r = 0;
    int ans = 1e9 + 5;
    seg.upd(interval[0][0], interval[0][1] - 1, 1);
    rep(l, n) {
        auto res = seg.query(1, m - 1);
        while (res.mn == 0 && r < n - 1) {
            r++;
            seg.upd(interval[r][0], interval[r][1] - 1, 1);
            res = seg.query(1, m - 1);
        }

        if (res.mn != 0) {
            dbg(l, r, res.mn);
            ans = min(ans, interval[r][2] - interval[l][2]);
        }

        seg.upd(interval[l][0], interval[l][1] - 1, -1);
    }
    cout << ans << "\n";
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
