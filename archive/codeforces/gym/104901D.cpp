#include <bits/stdc++.h>
using namespace std;

void solve() {
  long long la, ra, lb, rb;
  cin >> la >> ra >> lb >> rb;
  int ans = 0;
  for (int i = la; i <= min(100 + la, ra); i++) {
    for (int j = lb; j <= min(100 + lb, rb); j++) {
      long long res = i + j;
      for (auto &i : to_string(res)) {
        ans = max(ans, i - '0');
      }
    }
  }
  cout << ans << "\n";
}

int main() {
  int t;
  cin >> t;
  while (t--)
    solve();
}