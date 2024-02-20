#include <bits/stdc++.h>
using namespace std;

#define fi first
#define se second
#define end1 "\n"
#define pb push_back
#define eb emplace_back

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

#ifdef LOCAL
#include "/root/cp/templates/debug.h"
#else
#define debug(...) 42
#endif

void solve() {
	int n;
	cin >> n;
	//choc(n) = choc(n - 1) + (n + 2) + (n - 1)
	//choc(n) = choc(n - 1) + (2n + 1)
	//choc(4) = 26
	//choc(5) = 26 + 11
	//choc(6) = 26 + 11 + 13
	//choc(7) = 26 + 11 + 13 + 15
	ll diff = n - 4;
	ll base = 1LL * 11 * diff;
	ll add = (diff - 1) * diff;
	cout << 26 + base + add << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}