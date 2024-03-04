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

#define int long long

void solve() {
  int n, q;
  cin >> n >> q;

  vector<ll> c(n + 1), pref(n + 1), pref_ones(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> c[i];
    pref[i] += pref[i - 1] + c[i];
    pref_ones[i] += pref_ones[i - 1] + (c[i] == 1 ? 2 : 1);
  }

  dbg(pref);

  for (int i = 0; i < q; i++) {
    int l, r;
    cin >> l >> r;
    // if sum / l = 3 / 2 then can make half 1, half 2, OK
    // if sum / l > 3 / 2 then can make half 1, half 2, some 3 OK
    ll sum = pref[r] - pref[l - 1]; // this is invariant
    ll ones = pref_ones[r] - pref_ones[l - 1];
    if (sum >= ones && r - l + 1 >= 2) {
      cout << "YES\n";
    } else {
      cout << "NO\n";
    }
  }
}

signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}
