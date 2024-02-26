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

void solve() {}

int main() {
  cin.tie(0);
  ios::sync_with_stdio(false);

  int r, c;
  cin >> r >> c;
  vector<vector<int>> grid(r + 1, vector<int>(c + 1));
  for (int i = 1; i <= r; i++) {
    string s;
    cin >> s;
    for (int j = 1; j <= c; j++) {
      grid[i][j] = s[j - 1] - '0';
    }
  }

  // num_row needs to be number of 0's
  auto work = [](vector<vector<int>> &mat) {
    int num_r = mat.size() - 1;
    int num_c = mat[0].size() - 1;
    vector<vector<array<int, 2>>> pref_row(num_r + 1,
                                           vector<array<int, 2>>(num_c + 1));
    for (int i = 1; i <= num_r; i++) {
      for (int j = 1; j <= num_c; j++) {
        pref_row[i][j][0] += pref_row[i][j - 1][0] + (mat[i][j] == 0);
        pref_row[i][j][1] += pref_row[i][j - 1][1] + mat[i][j];
      }
    }

    vector<vector<vector<int>>> dp(
        num_r + 1,
        vector<vector<int>>(num_c + 1, vector<int>(num_c + 1, (int)1e9)));

    for (int l = 0; l <= num_c; l++) {
      for (int r = l; r <= num_c; r++) {
        dp[0][l][r] = 0;
      }
    }

    for (int row = 1; row <= num_r; row++) {
      for (int l = 1; l <= num_c; l++) {
        if (row == 1 && l > 1)
          break;
        int best = dp[row - 1][l - 1][l - 1]; // diag
                                              //
        for (int r = l; r <= num_c; r++) {
          best = min(best, dp[row - 1][l][r]);
          int form_row = pref_row[row][l - 1][1] - pref_row[row][0][1] +
                         pref_row[row][r][0] - pref_row[row][l - 1][0] +
                         pref_row[row][num_c][1] - pref_row[row][r][1];
          dp[row][l][r] = min(dp[row][l][r], best + form_row);
        }
      }

      // for each r, store best l such that ends on r
      for (int l = 1; l <= num_c; l++) {
        for (int r = l; r <= num_c; r++) {
          dp[row][l][r] = min(dp[row][l][r], dp[row][l - 1][r]);
        }
      }
    }

    return dp[num_r][num_c][num_c];
  };

  vector<vector<int>> transpose(c + 1, vector<int>(r + 1));
  for (int i = 0; i <= r; i++) {
    for (int j = 0; j <= c; j++) {
      transpose[j][i] = grid[i][j];
    }
  }

  if (r < c) {
    cout << work(transpose) << "\n";
  } else {
    cout << work(grid) << "\n";
  }
}
