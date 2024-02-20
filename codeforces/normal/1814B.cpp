#include <bits/stdc++.h>
using namespace std;

#define f first
#define s second

void setIO(string name) {
	cin.tie(nullptr)->sync_with_stdio(false);
	if (name.length() > 0) {
		freopen((name + ".in").c_str(), "r", stdin);
		freopen((name + ".out").c_str(), "w", stdout);
	}
}

int main() {
//	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int a, b;
		cin >> a >> b;
		long long ans = LLONG_MAX;
		for (int i = 0; i < 1 << 18; i++) {
			long long l = i + 1;
			long long p = a / l + (a % l != 0);
			long long q = b / l + (b % l != 0);
			ans = min(ans, i + p + q);
		}
		cout << ans << "\n";
	}
}