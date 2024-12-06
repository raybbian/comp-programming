#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using db = long double;

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

constexpr ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
constexpr ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

#ifdef LOCAL
#include "/home/rayb/comp-programming/debug/debug.hpp"
#define dbg(...) Debug(#__VA_ARGS__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

/**
 * Copied from https://gist.github.com/Chillee/4982ba0840745f63f3771bd84f280557#file-ntt-cpp
 */

template <int maxn> struct NTT {
    constexpr static int lg2(int n) { return 32 - __builtin_clz(n - 1); }
    const static int MAXN = 1 << lg2(maxn), MOD = 998244353, root = 3;
    int rev[MAXN], rt[MAXN];

    int mul(int a, int b) { return (long long)a * b % MOD; }
    int sub(int a, int b) { return b > a ? a - b + MOD : a - b; }
    int add(int a, int b) { return a + b >= MOD ? a + b - MOD : a + b; }

    int binExp(int base, long long exp) {
        if (exp == 0)
            return 1;
        return mul(binExp(mul(base, base), exp / 2), exp & 1 ? base : 1);
    }
    NTT() {
        rt[1] = 1;
        for (int k = 1; k < lg2(MAXN); k++) {
            int z[] = {1, binExp(root, (MOD - 1) >> (k + 1))};
            for (int i = (1 << k); i < 2 << k; i++)
                rt[i] = mul(rt[i / 2], z[i & 1]);
        }
    }
    void ntt(int *a, int n) {
        for (int i = 0; i < n; i++)
            rev[i] = (rev[i / 2] | (i & 1) << lg2(n)) / 2;
        for (int i = 0; i < n; i++)
            if (i < rev[i])
                swap(a[i], a[rev[i]]);
        for (int k = 1; k < n; k *= 2)
            for (int i = 0; i < n; i += 2 * k)
                for (int j = 0; j < k; j++) {
                    int z = mul(rt[j + k], a[i + j + k]);
                    a[i + j + k] = sub(a[i + j], z);
                    a[i + j] = add(a[i + j], z);
                }
    }
    int in[2][MAXN];
    vector<int> multiply(const vector<int> &a, const vector<int> &b) {
        if (a.empty() || b.empty())
            return {};
        int sz = a.size() + b.size() - 1, n = 1 << lg2(sz);
        fill(in[0], in[0] + n, 0), fill(in[1], in[1] + n, 0);
        copy(a.begin(), a.end(), in[0]), copy(b.begin(), b.end(), in[1]);
        ntt(in[0], n), ntt(in[1], n);
        int invN = binExp(n, MOD - 2);
        for (int i = 0; i < n; i++)
            in[0][i] = mul(mul(in[0][i], in[1][i]), invN);
        reverse(in[0] + 1, in[0] + n);
        ntt(in[0], n);
        return vector<int>(in[0], in[0] + sz);
    }
};

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

using mint = modint998;

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

void solve() {
    NTT<250005> ntt;
    Combo<250005> cmb;
    int n;
    cin >> n;
    vector<vector<int>> adj(n + 1);
    rep(i, n - 1) {
        int x, y;
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    vector<int> out(n + 1);
    vector<vector<int>> cur;
    rep(i, 1, n) {
        if (i == 1)
            out[i] = adj[i].size();
        else
            out[i] = adj[i].size() - 1;
        cur.push_back({1, out[i]});
    }

    while (cur.size() > 1) {
        vector<vector<int>> ncur;
        int s = cur.size();
        for (int i = 0; i + 1 < s; i += 2) {
            ncur.push_back(ntt.multiply(cur[i], cur[i + 1]));
        }
        if (s % 2 == 1)
            ncur.push_back(cur[s - 1]);
        cur = ncur;
    }

    mint par = 1;
    mint ans = 0;
    rep(i, 0, n - 1) {
        ans += par * cur.front()[i] * cmb.fact[n - i];
        par *= -1;
    }
    cout << ans << '\n';
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
