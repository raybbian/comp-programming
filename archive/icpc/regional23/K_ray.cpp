#include <bits/stdc++.h>
using namespace std;

#define ll long long

void solve() {
	ll n, k, a, b;
	cin >> n >> k >> a >> b;
	ll ans = 0;
	while(k > 0) {
		ll upd = min(k, ((b - a) * n) / a); //new serious runners
		assert(upd <= k);
		n += upd;
		k -= upd;
		ans++;
	}
	cout << ans << "\n";
}

int main() {
	int t;
	cin >> t;
	while(t--) solve();
}
