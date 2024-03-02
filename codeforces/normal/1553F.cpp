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
#define REP3(i, a, b) for (int i = (a); (a) <= (b) ? i <= (b) : i >= (a); (a) <= (b) ? i++ : i--)
#define GET_MACRO(a, b, c, d, ...) d
#define rep(...) GET_MACRO(__VA_ARGS__, REP3, REP2)(__VA_ARGS__)
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
template <typename T> struct PURQ { // cmb(ID,b) = b
    const T ID{0};
    T cmb(T a, T b) { return a + b; }
    int n;
    vector<T> seg;
    PURQ(int _n) {
        for (n = 1; n < _n;)
            n *= 2;
        seg.assign(2 * n, ID);
    }
    void pull(int p) { seg[p] = cmb(seg[2 * p], seg[2 * p + 1]); }
    void upd(int p, T val) { // set val at position p
        seg[p += n] = val;
        for (p /= 2; p; p /= 2)
            pull(p);
    }
    T query(int l, int r) { // zero-indexed, inclusive
        T ra = ID, rb = ID;
        for (l += n, r += n + 1; l < r; l /= 2, r /= 2) {
            if (l & 1)
                ra = cmb(ra, seg[l++]);
            if (r & 1)
                rb = cmb(seg[--r], rb);
        }
        return cmb(ra, rb);
    }
};

template <typename T> struct RUPQ {
    const T ID{0};
    T cmb(T a, T b) { return a + b; }
    int n;
    vector<T> seg;
    RUPQ(int _n) {
        for (n = 1; n < _n;)
            n *= 2;
        seg.assign(2 * n, ID);
    }
    void upd(int l, int r, T val) { // inclusive
        for (l += n, r += n + 1; l < r; l /= 2, r /= 2) {
            if (l & 1) {
                seg[l] = cmb(seg[l], val);
                l++;
            }
            if (r & 1) {
                r--;
                seg[r] = cmb(seg[r], val);
            }
        }
    }
    T query(int p) {
        T res = ID;
        for (p += n; p; p /= 2)
            res = cmb(res, seg[p]);
        return res;
    }
    void push() { // order of modification cannot affect result
        for (int i = 1; i < n; i++) {
            seg[2 * i] = cmb(seg[2 * i], seg[i]);
            seg[2 * i + 1] = cmb(seg[2 * i + 1], seg[i]);
            seg[i] = ID;
        }
    }
};

void solve() {
    int n;
    cin >> n;
    vector<ll> a(n + 1), ans(n + 1), pref(n + 1);
    ll max_a = 0;
    rep(i, 1, n) {
        cin >> a[i];
        pref[i] = pref[i - 1] + a[i];
        max_a = max(max_a, a[i]);
    }

    RUPQ<ll> s(max_a);
    PURQ<ll> t(max_a);

    rep(i, 1, n) {
        ll sum = 0;
        sum += a[i] * (i - 1); // a[k] * (k - 1)
        sum -= s.query(a[i]);  // penalties

        // this is OK because all a[i] distinct, cannot be more than one a[i] = 1
        for (int j = a[i]; j <= max_a; j += a[i]) { // for each interval of a[i]
            int l = j, r = min(max_a, j + a[i] - 1);
            s.upd(l, r, a[i] * (j / a[i])); // penalty for next number a[k] mod a[i]
        }
        ans[i] += sum;
    }

    rep(i, 1, n) {
        ll sum = 0;
        sum += pref[i - 1];

        for (int j = a[i]; j <= max_a; j += a[i]) {
            int l = j, r = min(max_a, j + a[i] - 1);
            sum -= a[i] * t.query(l, r) * (j / a[i]);
            // query is number of a[i] in this range
            // all in range contributes to a[i] * (nums in range) * (j / a[i]) penalty
            // number of a[i] that can fit in these a[i]
        }
        ans[i] += sum;
        t.upd(a[i], 1);
    }

    rep(i, 1, n) {
        ans[i] += ans[i - 1];
        cout << ans[i] << " \n"[i == n];
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
