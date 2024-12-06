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

inline bool is_full(int i, int j) { return j > i - 1; }
inline bool is_last_end(int i, int j) { return j == i - 1; }

void solve(vector<vector<pair<mint, mint>>> &dp) {
    ll n, k;
    cin >> n >> k;
    mint two = 2;

    int used = 0;
    mint consec = 0;
    mint ans = 0;
    per(i, bits(n), 0) {
        if (n & (1LL << i)) {
            int j = k - used;
            if (j < 0)
                break;
            consec += dp[i][j].first;
            dbg(consec, dp[i][j], is_full(i, j));
            if (!is_full(i, j)) {
                ans += consec * (consec + 1) / two;
                consec = 0;
            }
            dbg(consec, ans);
            ans += dp[i][j].second;
            used++;
        }
    }
    ans += consec * (consec + 1) / two;
    dbg(ans, consec, n, bits(n), k);
    cout << ans << '\n';
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    auto dp = ndvec<pair<mint, mint>>(61, 61, pair<mint, mint>{mint(0), mint(0)});
    mint two = 2;

    rep(i, 0, 60) { dp[i][0] = {1, 0}; }
    rep(i, 0, 60) { dp[0][i] = {1, 0}; }
    rep(i, 1, 60) rep(j, 1, 60) {
        mint consec = dp[i - 1][j].first;
        mint add = dp[i - 1][j - 1].second;
        if (is_full(i - 1, j)) {
            assert(dp[i - 1][j].second == 0);
            consec += dp[i - 1][j - 1].first;
        } else {
            add +=
                dp[i - 1][j - 1].first * (dp[i - 1][j - 1].first + 1) / two + dp[i - 1][j].second;
        }
        dp[i][j] = {consec, add};
    }

    int t;
    cin >> t;
    while (t--)
        solve(dp);
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
