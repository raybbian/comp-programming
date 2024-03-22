#include <bits/stdc++.h>
using namespace std;

#define ll long long

int main() {
  int n;
  cin >> n;
  vector<ll> has(n + 1), req(n + 1);
  for (int i = 0; i <= n; i++) {
    cin >> has[i];
  }
  for (int i = 0; i <= n; i++) {
    cin >> req[i];
  }

  ll ans = 0;
  for (int i = n; i > 0; i--) {
    int need = req[i] - has[i];
    if (need < 0)
      continue;
    ll split = (need + 1) / 2;
    has[i - 1] -= split;
    ans += split;
  }
  if (has[0] < req[0]) {
    cout << -1 << "\n";
  } else {
    cout << ans << "\n";
  }
}