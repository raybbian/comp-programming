#include <bits/stdc++.h>
using namespace std;

int main() {
  int n;
  cin >> n;
  string s;
  cin >> s;

  vector<array<int, 3>> pref(3 * n + 1, {0, 0, 0});
  for (int i = 1; i <= 3 * n; i++) {
    char c = s[i - 1];
    pref[i][c - 'A']++;
    for (int j = 0; j < 3; j++) {
      pref[i][j] += pref[i - 1][j];
    }
  }

  if (pref[3 * n][0] == pref[3 * n][1] && pref[3 * n][1] == pref[3 * n][2]) {
    cout << 0 << "\n";
    return 0;
  }

  int min_j = -1;
  int far_j = -1;
  int min_amt = INT_MAX;
  int far_amt = 0;
  for (int j = 0; j < 3; j++) {
    if (pref[3 * n][j] - n < min_amt) {
      min_amt = pref[3 * n][j] - n;
      min_j = j;
    }
    if (pref[3 * n][j] - n > far_amt) {
      far_amt = pref[3 * n][j] - n;
      far_j = j;
    }
  }
  int o_j = -1;
  for (int j = 0; j < 3; j++) {
    if (j != min_j && j != far_j) {
      o_j = j;
      break;
    }
  }

  // cerr << pref[3 * n][0] << " " << pref[3 * n][1] << " " << pref[3 * n][2]
  //      << "\n";
  //
  // check one operation
  // maintain this character's count (range - amount in range should be n)
  int l = 1;
  int r = 1;
  while (l <= r && r <= 3 * n) {
    while (l <= r && r - l + 1 - (pref[r][min_j] - pref[l - 1][min_j]) >
                         n - pref[3 * n][min_j]) {
      l++;
    }
    if (r - l + 1 - (pref[r][min_j] - pref[l - 1][min_j]) ==
        n - pref[3 * n][min_j]) {
      if ((pref[r][(min_j + 1) % 3] - pref[l - 1][(min_j + 1) % 3] ==
           pref[3 * n][(min_j + 1) % 3] - n) &&
          (pref[r][(min_j + 2) % 3] - pref[l - 1][(min_j + 2) % 3] ==
           pref[3 * n][(min_j + 2) % 3] - n)) {
        cout << 1 << "\n";
        cout << l << " " << r << " " << (char)(min_j + 'A') << "\n";
        return 0;
      }
    }

    r++;
  }

  // check two operations, add min_j to make far_j = 0
  l = 1;
  r = 1;
  while (l <= r) {
    // need to make far_j equal to 0, if amount subtracted not enough,
    while (l <= r &&
           pref[r][far_j] - pref[l - 1][far_j] > pref[3 * n][far_j] - n) {
      l++;
    }

    // cout << "lr: " << l << " " << r << " range " << r - l + 1 << " "
    //      << (pref[r][far_j] - pref[l - 1][far_j]) << " "
    //      << n - pref[3 * n][min_j] << "\n";
    if (pref[r][far_j] - pref[l - 1][far_j] == pref[3 * n][far_j] - n) {
      int minj_add = r - l + 1 - (pref[r][min_j] - pref[l - 1][min_j]);
      int o_j_sub = pref[r][o_j] - pref[l - 1][o_j];
      int new_minj = pref[3 * n][min_j] + minj_add;
      int new_oj = pref[3 * n][o_j] - o_j_sub;
      // b = o_j
      if (new_oj > n)
        continue;

      // consider it
      cout << 2 << "\n";
      cout << l << " " << r << " " << (char)(min_j + 'A') << "\n";
      cout << l << " " << l + (n - new_oj) - 1 << " " << (char)(o_j + 'A')
           << "\n";
      return 0;
    }
    r++;
  }

  assert(false);

  // worst case scenarai
  cout << 3 << "\n";
  cout << 1 << " " << n << " " << 'A' << "\n";
  cout << n + 1 << " " << 2 * n << " " << 'B' << "\n";
  cout << 2 * n + 1 << " " << 3 * n << " " << 'C' << "\n";
}