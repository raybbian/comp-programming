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
  int n;
  cin >> n;
  string temp;
  cin >> temp;
  vector<ll> a(n + 1);
  vector<ll> pref(n + 1);
  for (int i = 1; i <= n; i++) {
    a[i] = temp[i - 1] - '0';
    pref[i] = a[i] + pref[i - 1];
  }

  auto dc = [&](int l, int r, auto &&dc)
      -> tuple<ll, unordered_map<ll, int>, unordered_map<ll, int>> {
    unordered_map<ll, int> lmap, rmap;
    for (int i = l; i <= r; i++) {
      ll rsum = pref[i] - pref[l - 1];
      lmap[rsum - (i - l + 1)]++;
    }
    for (int i = l; i <= r; i++) {
      ll rsum = pref[r] - pref[i - 1];
      rmap[(r - i + 1) - rsum]++;
    }

    if (l == r) {
      return make_tuple((a[l] == 1), lmap, rmap);
    }

    int mid = (l + r) / 2;
    const auto &[l_ans, l_lmap, l_rmap] = dc(l, mid, dc);
    const auto &[r_ans, r_lmap, r_rmap] = dc(mid + 1, r, dc);
    ll ans = l_ans + r_ans;

    dbg(l, r, l_rmap, r_lmap);

    for (auto &i : l_rmap) {
      // key is size - sum
      ll cnt = i.se;
      auto other_it = r_lmap.find(i.fi);
      if (other_it != r_lmap.end()) {
        ll o_cnt = other_it->se;
        ans += cnt * o_cnt;
      }
    }

    return make_tuple(ans, lmap, rmap);
  };

  const auto &[ans, l, r] = dc(1, n, dc);
  cout << ans << "\n";
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}
