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
	//b1 = a1 mod n
	//b2 = a1 + a2 mod n
	//b3 = a1 + a2 + a3 mod n
	//n, n - 1, n - 2, n - 3, n - 4, n - 5, n - 6
	//0, n - 1, n - 3, n - 6, n - 3
	//b1 .. b2 = 0 .. n - 1
	//6, 5, 2, 3, 4, 1
	//0, 5, 1, 4, 2, 3
	//
	//7, 6, 2, 4, 
	//0, 6, 1, 5, 2, 4, 3
	//8, 7, 2, 5, 4, 3, 6, 1
	//0, 7, 1, 6, 2, 5, 3, 4
	//
	if (n > 1 && n % 2 == 1) {
		cout << -1 << end1;
		return;
	}
	if (n == 1) {
		cout << 1 << end1;
		return;
	}
	vector<int> b(n, 0);
	for(int i = 0; i < n; i++) {
		if (i % 2 == 0) {
			b[i] = i / 2;
		} else {
			b[i] = n - i / 2 - 1;
		}
	}
	debug(b);
	vector<int> ans;
	ans.push_back(n);
	for(int i = 1; i < n; i++) {
		int diff = (b[i] - b[i - 1] + n) % n;
		ans.push_back(diff);
	}
	for(auto &i: ans) {
		cout << i << " ";
	}
	cout << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int t;
	cin >> t;
	while(t--) {
		solve();
	}
}