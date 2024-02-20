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
	multiset<ll> nums;
	for(int i = 0; i < n; i++) {
		int a;
		cin >> a;
		nums.insert(a);
	}
	ll l = *nums.begin() * *next(nums.begin());
	ll r = *prev(nums.end()) * *prev(prev(nums.end()));
	cout << max(l, r) << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}