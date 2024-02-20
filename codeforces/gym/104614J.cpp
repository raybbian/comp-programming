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
  using T = pair<char, char>;
  vector<T> cards;

  auto cascade = [&](auto &&cascade, int check_to) -> void {
    dbg(cards);
    for (int i = cards.size() - 1; i >= check_to; i--) {
      if (i - 3 >= 0 && cards[i].fi == cards[i - 3].fi) {
        cards.erase(cards.begin() + i - 3, cards.begin() + i + 1);
        cascade(cascade, i - 3);
        return;
      }
    }
    for (int i = cards.size() - 1; i >= check_to; i--) {
      if (i - 3 >= 0 && cards[i].se == cards[i - 3].se) {
        cards.erase(cards.begin() + i);
        cards.erase(cards.begin() + i - 3);
        cascade(cascade, i - 3);
        return;
      }
    }
  };

  for (int i = 0; i < 52; i++) {
    string tmp;
    cin >> tmp;
    cards.emplace_back(tmp[0], tmp[1]);
    cascade(cascade, cards.size() - 1);
  }

  cout << cards.size() << " ";
  for (int i = 0; i < cards.size(); i++) {
    T card = cards[i];
    cout << card.fi << card.se << " \n"[i == cards.size() - 1];
  }
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}