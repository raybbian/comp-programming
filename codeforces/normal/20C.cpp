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

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> adj(n + 1);
  for (int i = 0; i < m; i++) {
    int a, b, w;
    cin >> a >> b >> w;
    adj[a].emplace_back(b, w);
    adj[b].emplace_back(a, w);
  }

  vector<long long> d(n + 1, LLONG_MAX);
  vector<int> par(n + 1);
  using T = pair<long long, int>;
  priority_queue<T, vector<T>, greater<T>> pq;
  pq.push({0, n});
  d[n] = 0;
  par[n] = -1;
  while (!pq.empty()) {
    ll c;
    int node;
    tie(c, node) = pq.top();
    pq.pop();

    if (c != d[node])
      continue;

    for (auto &i : adj[node]) {
      if (c + i.second < d[i.first]) {
        d[i.first] = c + i.second;
        par[i.first] = node;
        pq.push({d[i.first], i.first});
      }
    }
  }
  dbg(adj, d, par); // debugging the adjacency list, distances, and parent array

  if (d[1] == LLONG_MAX)
    cout << -1 << "\n";
  else {
    int cur = 1;
    while (cur != -1) {
      cout << cur << " ";
      cur = par[cur];
    }
    cout << "\n";
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}