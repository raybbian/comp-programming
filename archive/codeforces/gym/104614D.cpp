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
  string s;
  cin >> s;
  int n = s.length();
  vector<vector<int>> pref(26, vector<int>(n + 1));
  vector<char> nuc = {'A', 'T', 'G', 'C'};
  for (int i = 1; i <= n; i++) {
    pref[s[i - 1] - 'A'][i]++;
    for (char c : nuc) {
      pref[c - 'A'][i] += pref[c - 'A'][i - 1];
    }
  }

  int m;
  cin >> m;
  for (int i = 0; i < m; i++) {
    int s, e;
    cin >> s >> e;
    auto cmp = [&](char a, char b) -> bool {
      return pref[a - 'A'][e] - pref[a - 'A'][s - 1] >
             pref[b - 'A'][e] - pref[b - 'A'][s - 1];
    };
    vector<char> out(nuc.begin(), nuc.end());
    sort(out.begin(), out.end(), cmp);
    for (char c : out) {
      cout << c;
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