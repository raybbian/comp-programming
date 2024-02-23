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

#define int long long

void solve() {
  int n;
  cin >> n;
  vector<ll> a(n + 1), pref(n + 1), suff(n + 2);
  suff[0] = LLONG_MAX;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
    pref[i] += pref[i - 1] + a[i];
  }
  for (int i = n; i >= 1; i--) {
    suff[i] += suff[i + 1] + a[i];
  }

  // calculate which intervals are of same value
  vector<int> beg(n + 1), end(n + 1);
  int l = 1;
  while (l <= n) {
    int r = l;
    while (r + 1 <= n && a[r + 1] == a[l]) {
      r++;
    }
    for (int i = l; i <= r; i++) {
      beg[i] = l;
      end[i] = r;
    }
    l = r + 1;
  }

  dbg(a, beg, end, pref, suff);

  for (int i = 1; i <= n; i++) {
    // suff[it] - suff[i] >= a[i]
    //
    if (a[i - 1] > a[i]) {
      cout << 1 << " \n"[i == n];
      continue;
    }
    if (i < n && a[i + 1] > a[i]) {
      cout << 1 << " \n"[i == n];
      continue;
    }
    auto ok1 = [&](int num) -> bool { return suff[num] - suff[i] > a[i]; };
    auto ok2 = [&](int num) -> bool { return pref[num] - pref[i] > a[i]; };

    int ans = INT_MAX;
    if (beg[i - 1] - 1 >= 1) {
      int left = last_true(1LL, beg[i - 1] - 1, ok1);
      if (left != 0 && i - left > 0) {
        ans = min(ans, i - left);
      }
    }
    if (i < n && end[i + 1] + 1 <= n) {
      int right = first_true(end[i + 1] + 1, n, ok2);
      if (right != n + 1 && right - i > 0) {
        ans = min(ans, right - i);
      }
    }

    if (ans == INT_MAX) {
      cout << -1 << " \n"[i == n];
    } else {
      cout << ans << " \n"[i == n];
    }
  }

  // if left or right is greater, time = 1
  // if slime greater than sum on right or sum on left, time = -1
  // range prefix on either side, until sum greater is when can get eaten
}

signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}
