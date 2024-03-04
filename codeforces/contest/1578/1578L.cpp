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
  vector<ll> ans;
  vector<ll> sum;
  ll total = 0;

  DSU(int n, vector<ll> &c) {
    e = vector<int>(n, -1);
    // can be infinite weight, get to single node, leave
    ans = vector<ll>(n, LLONG_MAX);
    sum = vector<ll>(c.begin(), c.end());
    for (auto &i : c) {
      total += i;
    }
  }

  int get(int x) { return e[x] < 0 ? x : e[x] = get(e[x]); }

  bool is_same(int a, int b) { return get(a) == get(b); }

  int size(int x) { return -e[get(x)]; }

  bool unite(int x, int y, ll w) {
    x = get(x), y = get(y);
    if (x == y)
      return false;
    if (e[x] > e[y])
      swap(x, y);
    e[x] += e[y];
    e[y] = x;

    ans[x] = max(min(ans[x], w - (total - sum[x])),
                 min(ans[y], w - (total - sum[y])));
    sum[x] += sum[y];
    return true;
  }
};

void solve() {
  int n, m;
  cin >> n >> m;
  vector<ll> c(n + 1);
  ll total = 0;
  for (int i = 1; i <= n; i++) {
    cin >> c[i];
    total += c[i];
  }

  vector<tuple<ll, int, int>> edges;
  for (int i = 0; i < m; i++) {
    int a, b;
    ll w;
    cin >> a >> b >> w;
    edges.emplace_back(w, a, b);
  }

  sort(edges.begin(), edges.end(), greater<tuple<ll, int, int>>());

  DSU dsu(n + 1, c);
  for (const auto &[w, a, b] : edges) {
    dsu.unite(a, b, w);
  }

  ll ans = dsu.ans[dsu.get(1)];
  if (ans <= 0) {
    cout << -1 << "\n";
  } else {
    cout << dsu.ans[dsu.get(1)] << "\n";
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}
