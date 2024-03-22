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

#define int long long

// insert snippets below:

void solve() {
  int n, k;
  cin >> n >> k;
  int resp;

  int a12 = 0;
  int a23 = 0;
  int a13 = 0;

  cout << "or " << 1 << " " << 2 << "\n";
  cout.flush();
  cin >> resp;
  a12 += resp;

  cout << "and " << 1 << " " << 2 << "\n";
  cout.flush();
  cin >> resp;
  a12 += resp;

  cout << "or " << 2 << " " << 3 << "\n";
  cout.flush();
  cin >> resp;
  a23 += resp;

  cout << "and " << 2 << " " << 3 << "\n";
  cout.flush();
  cin >> resp;
  a23 += resp;

  cout << "or " << 3 << " " << 1 << "\n";
  cout.flush();
  cin >> resp;
  a13 += resp;

  cout << "and " << 3 << " " << 1 << "\n";
  cout.flush();
  cin >> resp;
  a13 += resp;

  int a2 = (a12 + a23 - a13) / 2;
  int a1 = a12 - a2;
  int a3 = a13 - a1;

  vector<int> rest(n + 1);
  rest[1] = a1;
  rest[2] = a2;
  rest[3] = a3;

  for (int i = 4; i <= n; i++) {
    cout << "or " << 1 << " " << i << "\n";
    cout.flush();
    cin >> resp;
    rest[i] += resp;

    cout << "and " << 1 << " " << i << "\n";
    cout.flush();
    cin >> resp;
    rest[i] += resp;
    rest[i] -= rest[1];
  }

  sort(rest.begin(), rest.end());
  cout << "finish " << rest[k] << "\n";
  cout.flush();
}

signed main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}
