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

using mint = modint998;

vector<mint> fact, inv;
void factorial(int n) {
    fact.assign(n + 1, 0);
    fact[0] = 1;
    rep(i, 1, n) { fact[i] = i * fact[i - 1]; }
}

void inverses(int n) {
    inv.assign(n + 1, 0);
    inv[n] = power(fact[n], 998244353 - 2);
    per(i, n, 1) { inv[i - 1] = inv[i] * i; }
}

mint multichoose(ll n, ll r) {
    mint res = 1;
    ll temp = n + r - 1;
    rep(i, r) {
        res *= temp;
        temp--;
    }
    res *= inv[r];
    return res;
}

void solve() {
    ll n, C;
    cin >> n >> C;
    vector<ll> val(n + 1), left(n + 1), right(n + 1);
    rep(i, 1, n) { cin >> left[i] >> right[i] >> val[i]; }

    vector<ll> inorder = {1};
    auto dfs = [&](int node, auto &&dfs) -> void {
        if (node == -1)
            return;
        dfs(left[node], dfs);
        inorder.push_back(val[node]);
        dfs(right[node], dfs);
    };
    dfs(1, dfs);
    inorder.push_back(C);

    dbg(inorder);
    int m = (int)inorder.size();
    mint ans = 1;
    rep(l, m) {
        if (inorder[l] != -1)
            continue;
        int r = l;
        while (r + 1 < m && inorder[r + 1] == -1)
            r++;

        int sz = r - l + 1;
        ll lb = inorder[l - 1];
        ll rb = inorder[r + 1];

        dbg(lb, rb, sz, multichoose(rb - lb + 1, sz));
        ans *= multichoose(rb - lb + 1, sz);

        l = r;
    }
    cout << ans.val() << ln;
}

signed main() {
    cin.tie(nullptr)->sync_with_stdio(false);
    factorial(500005);
    inverses(500005);
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
