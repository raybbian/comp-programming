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
	int n, t;
	cin >> n >> t;
	int dur[n];
	for(int i = 0; i < n; i++) {
		cin >> dur[i];
	}
	int ent[n];
	for(int i = 0; i < n; i++) {
		cin >> ent[i];
	}
	int ans = 0, out = -1;
	for(int i = 0; i < n; i++) {
		int time = dur[i] + i;
		if (time > t) continue;
		if (ent[i] > ans) {
			ans = ent[i];
			out = i + 1;
		}
	}
	cout << out << end1;
}

int main() {
	cin.tie(0)->sync_with_stdio(false);
	int q;
	cin >> q;
	while(q--) {
		solve();
	}
}