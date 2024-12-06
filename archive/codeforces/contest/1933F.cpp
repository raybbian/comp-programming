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
vector<pii> nxt = {{-1, 0}, {1, 0}, {0, 1}};

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> a(n, vector<int>(m));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> a[i][j];
    }
  }

  auto v = ndvec<bool>(n, m, n, false);
  queue<tuple<int, int, int>> q;
  q.push({0, 0, 0});
  v[0][0][0] = true;
  auto par = ndvec<pii>(n, m, pii{-1, -1});
  while (!q.empty()) {
    auto &[r, c, time] = q.front();
    q.pop();

    if (r == n - 1 && c == m - 1) {
      cout << time << "\n";
      return;
    }

    for (auto &i : nxt) {
      int new_r = (r + i.fi + n) % n;
      int new_c = (c + i.se + m) % m;
      if (v[new_r][new_c][(time + 1) % n])
        continue;
      if (a[(new_r + time + 1) % n][new_c])
        continue;
      if (i.fi == 1 && a[(r + time + 1) % n][new_c])
        continue;

      v[new_r][new_c][(time + 1) % n] = true;
      q.push({new_r, new_c, time + 1});
    }
  }

  cout << -1 << "\n";
  return;
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}
