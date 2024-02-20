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
  int n, k;
  cin >> n >> k;
  vector<int> a(n);
  int e_cnt = 0, o_cnt = 0;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    a[i] %= 2;
    if (a[i] == 0) {
      e_cnt++;
    } else {
      o_cnt++;
    }
  }

  int moves = n - k;
  if (moves == 0) {
    if (o_cnt % 2 == 1) {
      cout << "Stannis\n";
    } else {
      cout << "Daenerys\n";
    }
    return;
  }

  if (moves % 2 == 0) {
    // daenerys last move
    if (k % 2 == 0) {
      // result is even
      cout << "Daenerys\n";
    } else {
      if (e_cnt <= moves / 2) {
        cout << "Stannis\n";
      } else {
        cout << "Daenerys\n";
      }
    }
  } else {
    // stannis last move
    if (k % 2 == 0) {
      if (o_cnt <= moves / 2 || e_cnt <= moves / 2) {
        cout << "Daenerys\n";
      } else {
        cout << "Stannis\n";
      }
    } else {
      if (o_cnt <= moves / 2) {
        cout << "Daenerys\n";
      } else {
        cout << "Stannis\n";
      }
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