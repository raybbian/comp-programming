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

/**
 * Description: Mod integer class for doing modular arithmetic.
 * Source: https://github.com/jakobkogler/Algorithm-DataStructures/blob/master/Math/Modular.h
 * Verification: https://open.kattis.com/problems/modulararithmetic
 * Time: fast
 */

template <int MOD> struct ModInt {
    long long v;
    ModInt(long long _v = 0) {
        v = (-MOD < _v && _v < MOD) ? _v : _v % MOD;
        if (v < 0)
            v += MOD;
    }
    ModInt &operator+=(const ModInt &other) {
        v += other.v;
        if (v >= MOD)
            v -= MOD;
        return *this;
    }
    ModInt &operator-=(const ModInt &other) {
        v -= other.v;
        if (v < 0)
            v += MOD;
        return *this;
    }
    ModInt &operator*=(const ModInt &other) {
        v = v * other.v % MOD;
        return *this;
    }
    ModInt &operator/=(const ModInt &other) { return *this *= inverse(other); }
    bool operator==(const ModInt &other) const { return v == other.v; }
    bool operator!=(const ModInt &other) const { return v != other.v; }
    friend ModInt operator+(ModInt a, const ModInt &b) { return a += b; }
    friend ModInt operator-(ModInt a, const ModInt &b) { return a -= b; }
    friend ModInt operator*(ModInt a, const ModInt &b) { return a *= b; }
    friend ModInt operator/(ModInt a, const ModInt &b) { return a /= b; }
    friend ModInt operator-(const ModInt &a) { return 0 - a; }
    friend ModInt power(ModInt a, long long b) {
        ModInt ret(1);
        while (b > 0) {
            if (b & 1)
                ret *= a;
            a *= a;
            b >>= 1;
        }
        return ret;
    }
    friend ModInt inverse(ModInt a) { return power(a, MOD - 2); }
    friend istream &operator>>(istream &is, ModInt &m) {
        is >> m.v;
        m.v = (-MOD < m.v && m.v < MOD) ? m.v : m.v % MOD;
        if (m.v < 0)
            m.v += MOD;
        return is;
    }
    friend ostream &operator<<(ostream &os, const ModInt &m) { return os << m.v; }
};

using modint107 = ModInt<1000000007>;
using modint998 = ModInt<998244353>;

using mint = modint107;

template <int MAXN> struct Combo {
    mint fact[MAXN];
    mint inv[MAXN];
    Combo() {
        fact[0] = 1;
        for (int i = 1; i < MAXN; i++)
            fact[i] = fact[i - 1] * i;
        inv[MAXN - 1] = mint(1) / fact[MAXN - 1];
        for (int i = MAXN - 2; i >= 0; i--)
            inv[i] = inv[i + 1] * (i + 1);
    }

    mint choose(int n, int k) { return fact[n] * inv[k] * inv[n - k]; }
};

Combo<250005> cmb;

inline mint calc(pii a, pii b) {
    int left = b.second - a.second;
    int right = b.first - a.first;
    return cmb.choose(left + right, left);
}

void solve() {
    int h, w, n;
    cin >> h >> w >> n;
    vector<pii> pts(n + 2);
    rep(i, 1, n) { cin >> pts[i].first >> pts[i].second; }
    pts[0] = {1, 1};
    pts[n + 1] = {h, w};
    sort(pts.begin(), pts.end());

    auto dp = ndvec<mint>(n + 2, mint(0));
    dp[0] = 1;
    rep(i, 1, n + 1) {
        dp[i] = calc(pts[0], pts[i]);
        rep(j, 1, i - 1) {
            if (pts[j].first <= pts[i].first && pts[j].second <= pts[i].second) {
                dp[i] -= calc(pts[j], pts[i]) * dp[j];
            }
        }
    }
    dbg(dp);
    cout << dp[n + 1] << '\n';
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
