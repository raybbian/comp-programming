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

struct HLD {
    vector<int> parent, depth, heavy, head, pos;
    int cur_pos;
    PURQ<ll> seg;

    HLD(vector<vector<int>> const &adj) : seg(adj.size()) {
        int n = adj.size();
        parent = vector<int>(n);
        depth = vector<int>(n);
        heavy = vector<int>(n, -1);
        head = vector<int>(n);
        pos = vector<int>(n);
        cur_pos = 0;

        dfs(0, adj);
        decompose(0, 0, adj);
    }

    int dfs(int v, vector<vector<int>> const &adj) {
        int size = 1;
        int max_c_size = 0;
        for (int c : adj[v]) {
            if (c != parent[v]) {
                parent[c] = v, depth[c] = depth[v] + 1;
                int c_size = dfs(c, adj);
                size += c_size;
                if (c_size > max_c_size)
                    max_c_size = c_size, heavy[v] = c;
            }
        }
        return size;
    }

    void decompose(int v, int h, vector<vector<int>> const &adj) {
        head[v] = h, pos[v] = cur_pos++;
        if (heavy[v] != -1)
            decompose(heavy[v], h, adj);
        for (int c : adj[v]) {
            if (c != parent[v] && c != heavy[v])
                decompose(c, c, adj);
        }
    }

    void upd(int a, ll x) { // assignment
        seg.upd(pos[a], x);
    }

    ll query(int a, int b) {
        ll res = 0;
        for (; head[a] != head[b]; b = parent[head[b]]) {
            if (depth[head[a]] > depth[head[b]])
                swap(a, b);
            res += seg.query(pos[head[b]], pos[b]);
        }
        if (depth[a] > depth[b])
            swap(a, b);
        res += seg.query(pos[a], pos[b]);
        return res;
    }
};

void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    vector<vector<int>> adj(n);

    rep(i, n) {
        cin >> a[i];
        a[i] = abs(a[i]);
    }
    rep(i, n - 1) {
        int s, t;
        cin >> s >> t;
        s--, t--;
        adj[s].push_back(t);
        adj[t].push_back(s);
    }

    HLD hld(adj);
    rep(i, n) hld.upd(i, 2 * a[i]);

    rep(i, q) {
        int type, u, c;
        cin >> type >> u >> c;
        if (type == 1) {
            u--;
            a[u] = abs(c);
            hld.upd(u, 2 * a[u]);
        } else {
            u--, c--;
            cout << hld.query(u, c) - a[u] - a[c] << "\n";
        }
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
