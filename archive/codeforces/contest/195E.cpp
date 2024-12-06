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
#define PER2(i, n) for (int i = (n) - 1; i >= 0; i--)
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
#include "/home/rayb/comp-programming/debug/debug.hpp"
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

template <typename T> struct SegmentTree {
    int n;
    vector<T> a, st, lazy;

    SegmentTree(int _n) : n(_n), st(4 * n), lazy(4 * n) {}

    SegmentTree(const vector<T> &_a) : n((int)_a.size()), a(_a), st(4 * n), lazy(4 * n) {
        build(1, 0, n - 1);
    }

    void build(int p, int l, int r) {
        if (l == r) {
            st[p] += a[l];
            return;
        }
        int m = (l + r) / 2;
        build(2 * p, l, m);
        build(2 * p + 1, m + 1, r);
        st[p] = st[2 * p] + st[2 * p + 1];
    }

    void push(int p, int l, int r) {
        if (lazy[p] != 0) {
            st[p] += (r - l + 1) * lazy[p];
            if (l != r) {
                lazy[2 * p] += lazy[p];
                lazy[2 * p + 1] += lazy[p];
            }
            lazy[p] = 0;
        }
    }

    T query(int p, int l, int r, int i, int j) {
        push(p, l, r);
        if (i > r || j < l)
            return 0;
        if (i <= l && r <= j)
            return st[p];
        int m = (l + r) / 2;
        return query(2 * p, l, m, i, j) + query(2 * p + 1, m + 1, r, i, j);
    }

    T query(int i, int j) { return query(1, 0, n - 1, i, j); }

    void update(int p, int l, int r, int i, int j, T val) {
        push(p, l, r);
        if (i > r || j < l)
            return;
        if (i <= l && r <= j) {
            st[p] += (r - l + 1) * val;
            if (l != r) {
                lazy[2 * p] += val;
                lazy[2 * p + 1] += val;
            }
            return;
        }
        int m = (l + r) / 2;
        update(2 * p, l, m, i, j, val);
        update(2 * p + 1, m + 1, r, i, j, val);
        st[p] = st[2 * p] + st[2 * p + 1];
    }

    void update(int i, int j, T val) { update(1, 0, n - 1, i, j, val); }
};

template <typename T, bool VAL_IN_EDGES> struct HLD {
    int n, ti;
    vector<int> par, sz, depth, root, pos;
    vector<vector<int>> adj;
    SegmentTree<T> st;

    HLD(int _n) : n(_n), ti(0), par(n, -1), sz(n, 1), depth(n), root(n), pos(n), adj(n), st(n) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void init(int r = -1) {
        if (r == -1) {
            for (int u = 0; u < n; u++)
                if (par[u] == -1) {
                    root[u] = u;
                    dfsSz(u);
                    dfsHld(u);
                }
        } else {
            root[r] = r;
            dfsSz(r);
            dfsHld(r);
        }
    }

    void dfsSz(int u) {
        if (par[u] != -1)
            adj[u].erase(find(adj[u].begin(), adj[u].end(), par[u]));
        for (int &v : adj[u]) {
            par[v] = u;
            depth[v] = depth[u] + 1;
            dfsSz(v);
            sz[u] += sz[v];
            if (sz[v] > sz[adj[u][0]])
                swap(v, adj[u][0]);
        }
    }

    void dfsHld(int u) {
        pos[u] = ti++;
        for (int v : adj[u]) {
            root[v] = (v == adj[u][0] ? root[u] : v);
            dfsHld(v);
        }
    }

    template <class B> void process(int u, int v, B op) {
        for (; root[u] != root[v]; u = par[root[u]]) {
            if (depth[root[u]] < depth[root[v]])
                swap(u, v);
            op(pos[root[u]], pos[u]);
        }
        if (depth[u] > depth[v])
            swap(u, v);
        if (pos[u] + VAL_IN_EDGES <= pos[v])
            op(pos[u] + VAL_IN_EDGES, pos[v]);
    }

    // path
    T query(int u, int v) {
        T ret = 0;
        process(u, v, [this, &ret](int l, int r) { ret += st.query(l, r); });
        return ret;
    }

    void update(int u, int v, T val) {
        process(u, v, [this, &val](int l, int r) { st.update(l, r, val); });
    }

    // subtree
    T query(int u) { return st.query(pos[u] + VAL_IN_EDGES, pos[u] + sz[u] - 1); }

    void update(int u, T val) { st.update(pos[u] + VAL_IN_EDGES, pos[u] + sz[u] - 1, val); }
};

void solve() {
    int n;
    cin >> n;

    vector<int> lead(n + 1, -1);
    auto get = [&](int x, auto &&get) -> int {
        return lead[x] < 0 ? x : lead[x] = get(lead[x], get);
    };

    vector<vector<pair<int, mint>>> to_n(n + 1);
    HLD<mint, true> hld(n + 1);
    rep(i, 1, n) {
        int k;
        cin >> k;
        rep(j, k) {
            int v;
			mint x;
            cin >> v >> x;
            to_n[i].emplace_back(v, x);
            int root = get(v, get);
            lead[root] = i;
            hld.addEdge(i, root);
			dbg(i, root);
        }
    }

    lead = vector<int>(n + 1, -1);
    mint ans = 0;
    hld.init();
    rep(i, 1, n) {
        int k = to_n[i].size();
        rep(j, k) {
            auto [v, x] = to_n[i][j];
            int root = get(v, get);
            lead[root] = i;
            mint amt = hld.query(v, root);
            mint new_edge = amt + x;
            dbg(amt, x, new_edge, v, root, i);
            hld.update(i, root, new_edge);
            ans += new_edge;
        }
    }
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
