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

// n-dimensional vector
template <typename T> vector<T> ndvec(size_t size, T initial_value) {
  return vector<T>(size, initial_value);
}
template <typename T, typename... U> auto ndvec(size_t head, U &&...u) {
  auto inner = ndvec<T>(u...);
  return vector<decltype(inner)>(head, inner);
}

// bitwise ops
constexpr int popcnt(int x) { return __builtin_popcount(x); } // # of bits set
constexpr int bits(int x) {
  return x == 0 ? 0 : 31 - __builtin_clz(x);
} // floor(log2(x))

// better math
ll c_div(ll a, ll b) { return a / b + ((a ^ b) > 0 && a % b); }
ll f_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

// check and assign max and min
template <typename T> bool chk_min(T &a, const T &b) {
  return b < a ? a = b, 1 : 0;
}
template <typename T> bool chk_max(T &a, const T &b) {
  return a < b ? a = b, 1 : 0;
}

#ifdef LOCAL
#include "/home/rayb/Documents/comp-programming/debug/debug.hpp"
#define dbg(...)                                                               \
  Debug(#__VA_ARGS__, sizeof(#__VA_ARGS__), __LINE__, __FUNCTION__, __VA_ARGS__)
#else
#define dbg(...) 42
#endif

// insert snippets below:

struct DSU {
  vector<int> e;
  DSU(int n) { e = vector<int>(n, -1); }

  int get(int x) { return e[x] < 0 ? x : e[x] = get(e[x]); }

  bool is_same(int a, int b) { return get(a) == get(b); }

  int size(int x) { return -e[get(x)]; }

  bool unite(int x, int y) {
    x = get(x), y = get(y);
    if (x == y)
      return false;
    if (e[x] > e[y])
      swap(x, y);
    e[x] += e[y];
    e[y] = x;
    return true;
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<pii>> adj(n + 1);
  vector<DSU> dsu(31, DSU(n + 1));
  vector<DSU> to_even(31, DSU(n + 2));

  for (int i = 0; i < m; i++) {
    int a, b;
    long long w;
    cin >> a >> b >> w;
    adj[a].emplace_back(b, w);
    adj[b].emplace_back(a, w);
    for (int i = 0; i <= 30; i++) {
      long long mask = (1LL << i);
      if ((w & mask) != 0) {
        // has bit set
        dsu[i].unite(a, b);
        if (i != 0) {
          to_even[i].unite(a, b);
        }
      }
      if (w % 2 == 0) {
        to_even[i].unite(n + 1, a);
        to_even[i].unite(n + 1, b);
      }
    }
  }

  // bfs from each node? find path with

  int q;
  cin >> q;
  for (int i = 0; i < q; i++) {
    int u, v;
    cin >> u >> v;
    bool found = false;
    for (int i = 0; i <= 30 && !found; i++) {
      // check if single bit path
      if (dsu[i].is_same(u, v)) {
        cout << 0 << "\n";
        found = true;
      }
    }
    for (int i = 0; i <= 30 && !found; i++) {
      if (to_even[i].is_same(u, n + 1)) {
        cout << 1 << "\n";
        found = true;
      }
    }
    if (!found) {
      cout << 2 << "\n";
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