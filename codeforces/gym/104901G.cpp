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
//

// custom hash
struct chash {
  static uint64_t splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
  }
  size_t operator()(uint64_t x) const {
    static const uint64_t FIXED_RANDOM =
        chrono::steady_clock::now().time_since_epoch().count();
    return splitmix64(x + FIXED_RANDOM);
  }
  template <typename T, typename U>
  size_t operator()(const pair<T, U> &x) const {
    uint64_t a = (*this)(x.fi);
    uint64_t b = (*this)(x.se);
    return a + 3 * b;
  }
};

void solve() {
  int r, c;
  cin >> r >> c;
  vector<vector<int>> grid(r, vector<int>(c));
  for (int i = 0; i < r; i++) {
    string s;
    cin >> s;
    for (int j = 0; j < c; j++) {
      grid[i][j] = (s[j] - '0');
    }
  }

  vector<unordered_set<pii, chash>> adj(r);
  unordered_map<int, unordered_map<int, int, chash>, chash> edge_t;

  int lc = 0, rc = c - 1;
  while (lc <= rc) {
    // count number of 1s in lc and rc
    if (lc == rc) {
      int cnt = 0;
      for (int i = 0; i < r; i++) {
        cnt += (grid[i][lc] == 1);
      }
      if (cnt > 1) {
        cout << 0 << "\n";
        return;
      }
    } else {
      vector<int> l_cnt, r_cnt;
      for (int i = 0; i < r; i++) {
        if (grid[i][lc] == 1) {
          l_cnt.push_back(i);
        }
        if (grid[i][rc] == 1) {
          r_cnt.push_back(i);
        }
      }

      if (l_cnt.size() + r_cnt.size() > 2) { // more than two here
        cout << 0 << "\n";
        return;
      }

      if (l_cnt.size() == 1 && r_cnt.size() == 1) {
        if (edge_t[l_cnt[0]][r_cnt[0]] == 1) {
          cout << 0 << "\n";
          return;
        }

        adj[l_cnt[0]].insert({r_cnt[0], 0});
        adj[r_cnt[0]].insert({l_cnt[0], 0});

        edge_t[l_cnt[0]][r_cnt[0]] = -1;
        edge_t[r_cnt[0]][l_cnt[0]] = -1;
      } else if (l_cnt.size() == 2) {
        if (edge_t[l_cnt[0]][l_cnt[1]] == -1) {
          cout << 0 << "\n";
          return;
        }

        adj[l_cnt[0]].insert({l_cnt[1], 1});
        adj[l_cnt[1]].insert({l_cnt[0], 1});

        edge_t[l_cnt[0]][l_cnt[1]] = 1;
        edge_t[l_cnt[1]][l_cnt[0]] = 1;
      } else if (r_cnt.size() == 2) {
        if (edge_t[r_cnt[0]][r_cnt[1]] == -1) {
          cout << 0 << "\n";
          return;
        }

        adj[r_cnt[0]].insert({r_cnt[1], 1});
        adj[r_cnt[1]].insert({r_cnt[0], 1});

        edge_t[r_cnt[0]][r_cnt[1]] = 1;
        edge_t[r_cnt[1]][r_cnt[0]] = 1;
      }
    }
    lc++;
    rc--;
  }

  queue<pii> q;
  vector<int> col(r, -1);
  long long ans = 1;
  for (int i = 0; i < r; i++) {
    if (col[i] != -1) {
      continue;
    }
    ans *= 2;
    ans %= 1000000007;
    q.push({i, 0});
    col[i] = 0;
    while (!q.empty()) {
      auto &[node, c] = q.front();
      q.pop();

      for (auto &i : adj[node]) {
        int new_col = (col[node] + i.second) % 2;

        if (col[i.first] != -1 && col[i.first] != new_col) {
          cout << 0 << "\n";
          return;
        }
        if (col[i.first] == -1) {
          col[i.first] = new_col;
          q.push({i.first, new_col});
        }
      }
    }
  }
  cout << ans << "\n";
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  int t;
  cin >> t;
  while (t--)
    solve();
}