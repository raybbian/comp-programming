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
constexpr int bitsll(ll x) { return x == 0 ? 0 : 63 - __builtin_clzll(x); }

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
template <typename T> constexpr T power(T a, ll b) {
    T res = 1;
    for (; b; b /= 2, a *= a) {
        if (b % 2)
            res *= a;
    }
    return res;
}

template <int P> struct MInt {
    int x;
    constexpr MInt() : x{} {}
    constexpr MInt(ll x) : x{norm(x % P)} {}

    constexpr int norm(int x) const {
        if (x < 0) {
            x += P;
        }
        if (x >= P) {
            x -= P;
        }
        return x;
    }
    constexpr int val() const { return x; }
    explicit constexpr operator int() const { return x; }
    constexpr MInt operator-() const {
        MInt res;
        res.x = norm(P - x);
        return res;
    }
    constexpr MInt inv() const {
        assert(x != 0);
        return power(*this, P - 2);
    }
    constexpr MInt &operator*=(MInt rhs) {
        x = 1LL * x * rhs.x % P;
        return *this;
    }
    constexpr MInt &operator+=(MInt rhs) {
        x = norm(x + rhs.x);
        return *this;
    }
    constexpr MInt &operator-=(MInt rhs) {
        x = norm(x - rhs.x);
        return *this;
    }
    constexpr MInt &operator/=(MInt rhs) { return *this *= rhs.inv(); }
    friend constexpr MInt operator*(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res *= rhs;
        return res;
    }
    friend constexpr MInt operator+(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res += rhs;
        return res;
    }
    friend constexpr MInt operator-(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res -= rhs;
        return res;
    }
    friend constexpr MInt operator/(MInt lhs, MInt rhs) {
        MInt res = lhs;
        res /= rhs;
        return res;
    }
    friend constexpr std::istream &operator>>(std::istream &is, MInt &a) {
        ll v;
        is >> v;
        a = MInt(v);
        return is;
    }
    friend constexpr std::ostream &operator<<(std::ostream &os, const MInt &a) {
        return os << a.val();
    }
    friend constexpr bool operator==(MInt lhs, MInt rhs) { return lhs.val() == rhs.val(); }
    friend constexpr bool operator!=(MInt lhs, MInt rhs) { return lhs.val() != rhs.val(); }
};

using modint107 = MInt<1000000007>;
using modint998 = MInt<998244353>;

using mint = modint107;

// custom hash
struct chash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
    template <typename T, typename U> size_t operator()(const pair<T, U> &x) const {
        uint64_t a = (*this)(x.fi);
        uint64_t b = (*this)(x.se);
        return a + 3 * b;
    }
};

int col_to_int(string &col) {
    if (col == "white")
        return 0;
    if (col == "yellow")
        return 1;
    if (col == "blue")
        return 2;
    if (col == "green")
        return 3;
    if (col == "red")
        return 4;
    if (col == "orange")
        return 5;
    assert(69 == 420);
}

const vector<vector<int>> ok = {
    {2, 3, 4, 5}, {2, 3, 4, 5}, {0, 1, 4, 5}, {0, 1, 4, 5}, {0, 1, 2, 3}, {0, 1, 2, 3},
};

void solve() {
    ll k, n;
    cin >> k >> n;
    unordered_map<ll, set<ll>, chash> adj;
    unordered_map<ll, int, chash> assign;
    rep(i, n) {
        ll ind;
        string col;
        cin >> ind >> col;

        assign[ind] = col_to_int(col);
        ll temp = ind;
        while (temp / 2 > 0) {
            adj[temp / 2].insert(temp);
            temp /= 2;
        }
    }

    int used = 0;
    unordered_map<ll, array<mint, 6>, chash> dp;
    auto dfs = [&](ll node, auto &&dfs) -> void {
        used++;
        if (!adj.count(node)) {
            rep(col, 6) {
                if (assign.count(node) && assign[node] != col)
                    continue;
                dp[node][col] = 1;
            }
            return;
        }
        each(next, adj[node]) { dfs(next, dfs); }
        if ((int)adj[node].size() == 2) {
            ll l_node = *adj[node].begin();
            ll r_node = *adj[node].rbegin();
            rep(m_col, 6) {
                if (assign.count(node) && m_col != assign[node])
                    continue;
                each(l_col, ok[m_col]) each(r_col, ok[m_col]) {
                    dp[node][m_col] += dp[l_node][l_col] * dp[r_node][r_col];
                }
            }
        } else {
            ll n_node = *adj[node].begin();
            rep(m_col, 6) {
                if (assign.count(node) && m_col != assign[node])
                    continue;
                each(n_col, ok[m_col]) { dp[node][m_col] += dp[n_node][n_col]; }
            }
        }
    };
    dfs(1LL, dfs);

    mint ans = 0;
    ll left = (1LL << k) - 1 - used;
    rep(col, 6) {
        if (assign.count(1) && assign[1] != col)
            continue;
        ans += dp[1][col];
    }
    mint base = 4;
    ans *= power(base, left);

    cout << ans << ln;
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
