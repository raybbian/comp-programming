#include <bits/stdc++.h>
using namespace std;

// typing is hard
using ll = long long;
using db = double;
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

const double PI = acos(-1);
//
//
void solve() {
  int n, k;
  cin >> n >> k;
  vector<pdd> points(n);
  vector<db> dist(n);
  vector<db> ang(n);
  for (int i = 0; i < n; i++) {
    cin >> points[i].fi >> points[i].se;
    dist[i] = sqrt(points[i].fi * points[i].fi + points[i].se * points[i].se);
    ang[i] = atan2(points[i].se, points[i].fi);
  }

  db l_search = 0;
  db r_search = 2e5;
  for (int i = 0; i < 35; i++) {
    db radius = l_search + (r_search - l_search) / 2;

    vector<pair<db, int>> process; // point, thing in, thing out

    int ans = 0;
    int in = 0;

    for (int i = 0; i < n; i++) {
      if (dist[i] > 2 * radius)
        continue;
      db alpha = acos(dist[i] / radius / 2);
      db l = ang[i] - alpha, r = ang[i] + alpha;
      if (l < 0)
        l += 2 * PI;
      if (r < 0)
        r += 2 * PI;
      process.emplace_back(l, 1);
      process.emplace_back(r, -1);
      if (l > r)
        in++;
    }

    sort(process.begin(), process.end());

    for (auto &i : process) {
      in += i.se;
      ans = max(in, ans);
    }

    if (ans < k) {
      l_search = radius;
    } else {
      r_search = radius;
    }
  }

  if (fabs(2e5 - l_search) < 1e-5) {
    cout << fixed << setprecision(14) << 0.0 << "\n";
  } else {
    cout << fixed << setprecision(14) << l_search << "\n";
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}
