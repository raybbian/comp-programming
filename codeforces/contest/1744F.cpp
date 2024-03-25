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

void solve() {
    // given l, r, count how many segments which has median <= mex
    int n;
    cin >> n;
    vector<int> a(n), ind(n);
    rep(i, n) {
        cin >> a[i];
        ind[a[i]] = i;
    }
    // mex > med if more than half contribute to mex
    set<int> nums;
    int lb, rb;
    lb = rb = ind[0];
    nums.insert(0);
    ll ans = 1;
    rep(i, 1, n) {
        while (nums.count(i) && i < n)
            i++;

        int nlb = max(0, min(lb, ind[i]));
        int nrb = min(n - 1, max(rb, ind[i]));
        if (nlb == lb) {
            rep(j, rb, nrb - 1) {
                int bestlb = max(0, j - 2 * i + 1);
                int add = lb - bestlb + 1;
                if (add <= 0)
                    continue;
                ans += add;
            }
        } else {
            per(j, lb, nlb + 1) {
                int bestrb = min(n - 1, j + 2 * i - 1);
                int add = bestrb - rb + 1;
                if (add <= 0)
                    continue;
                ans += add;
            }
        }

        if (i == n)
            break;
        per(j, lb - 1, nlb) { nums.insert(a[j]); }
        rep(j, rb + 1, nrb) { nums.insert(a[j]); }
        lb = nlb;
        rb = nrb;
    }
    cout << ans << ln;
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
