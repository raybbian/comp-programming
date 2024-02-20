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
vector<pii> orth = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

void solve() {
  int n, m, k;
  cin >> n >> m >> k;
  auto pics = ndvec<int>(k + 1, n, m, 0);
  for (int i = 0; i < k + 1; i++) {
    for (int r = 0; r < n; r++) {
      string row;
      cin >> row;
      for (int c = 0; c < m; c++) {
        pics[i][r][c] = row[c] - '0';
      }
    }
  }

  auto check = ndvec<bool>(n, m, true);
  for (int r = 1; r < n - 1; r++) {
    for (int c = 1; c < n - 1; c++) {
      int z_cnt = 0, o_cnt = 0;
      for (int i = 0; i < k + 1; i++) {
        if (pics[i][r][c] == 0) {
          z_cnt++;
        } else {
          o_cnt++;
        }
      }
      if (z_cnt == 0 || o_cnt == 0) {
        check[r][c] = false;
      }
    }
  }

  dbg(pics);

  auto before = ndvec<pair<int, unordered_set<int>>>(
      n, m, pair<int, unordered_set<int>>{-1, unordered_set<int>()});

  for (int i = 0; i < k + 1; i++) {
    for (int r = 1; r < n - 1; r++) {
      for (int c = 1; c < m - 1; c++) {
        if (!check[r][c])
          continue;
        bool is_surround = (pics[i][r - 1][c] == pics[i][r][c - 1] &&
                            pics[i][r - 1][c] == pics[i][r][c + 1] &&
                            pics[i][r + 1][c] == pics[i][r][c + 1]);
        int val = pics[i][r - 1][c];

        // all nonval in this position must appear before val
        if (is_surround) {
          dbg(r, c, before[r][c], val);
          assert(before[r][c].fi == -1 || before[r][c].fi == (val == 0));
          before[r][c].fi = (val == 0);
          if (val != pics[i][r][c]) {
            before[r][c].se.insert(i);
          }
        }
      }
    }
  }

  vector<unordered_set<int>> adj(k + 1);
  vector<int> in_deg(k + 1);

  for (int r = 1; r < n - 1; r++) {
    for (int c = 1; c < m - 1; c++) {
      if (!check[r][c])
        continue;
      for (int i = 0; i < k + 1; i++) {
        for (int j = i + 1; j < k + 1; j++) {
          if (before[r][c].se.count(i) && !before[r][c].se.count(j) &&
              !adj[i].count(j)) {
            adj[i].insert(j);
            in_deg[j]++;
          } else if (before[r][c].se.count(j) && !before[r][c].se.count(i) &&
                     !adj[j].count(i)) {
            adj[j].insert(i);
            in_deg[i]++;
          }
        }
      }
    }
  }

  queue<int> q;
  for (int i = 0; i < k + 1; i++) {
    if (in_deg[i] == 0)
      q.push(i);
  }

  vector<int> topo;
  while (!q.empty()) {
    int node = q.front();
    q.pop();

    topo.push_back(node);

    for (auto &i : adj[node]) {
      in_deg[i]--;
      if (in_deg[i] == 0) {
        q.push(i);
      }
    }
  }

  assert(topo.size() == k + 1);
  dbg(adj, topo);

  stringstream os;
  int cnt = 0;
  for (int i = 1; i < k + 1; i++) {
    // compare with previous
    for (int r = 1; r < n - 1; r++) {
      for (int c = 1; c < m - 1; c++) {
        if (pics[topo[i]][r][c] != pics[topo[i - 1]][r][c]) {
          os << 1 << " " << r + 1 << " " << c + 1 << "\n";
          cnt++;
        }
      }
    }
    os << 2 << " " << topo[i] + 1 << "\n";
    cnt++;
  }
  cout << topo[0] + 1 << "\n" << cnt << "\n" << os.str();
}

int main() {
  cin.tie(nullptr)->sync_with_stdio(false);
  // int t;
  // cin >> t;
  // while (t--)
  solve();
}