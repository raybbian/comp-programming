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
  ll n, k;
  cin >> n >> k;
  deque<pll> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i].fi;
  }
  for (int i = 0; i < n; i++) {
    cin >> a[i].se;
  }

  sort(a.begin(), a.end(),
       [](pll &a, pll &b) { return abs(a.se) < abs(b.se); });

  dbg(a);

  for (int i = 0; !a.empty(); i++) {
    ll count = k;
    if (abs(a.front().se) <= i) {
      cout << "NO\n";
      return;
    }
    while (count > 0 && !a.empty()) {
      ll rem = min(count, a.front().fi);
      a.front().fi -= rem;
      count -= rem;
      if (a.front().fi == 0) {
        a.pop_front();
      }
    }
  }

  cout << "YES\n";
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}
