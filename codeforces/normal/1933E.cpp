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

// insert snippets below

// binary search
template <typename T, typename U> T last_true(T l, T r, U f) {
  l--;
  assert(l <= r);
  while (l < r) {
    T mid = l + (r - l + 1) / 2;
    f(mid) ? l = mid : r = mid - 1;
  }
  return l;
}
template <typename T, typename U> T first_true(T l, T r, U f) {
  r++;
  assert(l <= r);
  while (l < r) {
    T mid = l + (r - l) / 2;
    f(mid) ? r = mid : l = mid + 1;
  }
  return l;
}
void solve() {
  int n;
  cin >> n;
  vector<ll> a(n + 1), pref(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    pref[i] = pref[i - 1] + a[i];
  }

  int q;
  cin >> q;
  while (q--) {
    int l, u;
    cin >> l >> u;
    auto ok = [&](int r) -> bool {
      ll p_segs = pref[r - 1] - pref[l - 1];
      ll p_gain = u * p_segs;
      ll p_loss = (p_segs - 1) * (p_segs) / 2;

      ll c_gain = u * (p_segs + a[r]);
      ll c_loss = (p_segs + a[r] - 1) * (p_segs + a[r]) / 2;
      dbg(l, r, u, c_gain, c_loss, p_gain, p_loss);
      if (c_gain - c_loss < 0)
        return false;
      return c_gain - c_loss > p_gain - p_loss;
    };

    ll ans = last_true(l, n, ok);
    if (ans == l - 1) {
      cout << l << " ";
    } else {
      cout << ans << " ";
    }
  }
  cout << "\n";
}

signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  ll t;
  cin >> t;
  while (t--)
    solve();
}
