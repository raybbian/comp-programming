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
	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int n, m;
		cin >> n >> m;
		int cards[m + 1][n + 1];
		for (int i = 0; i < n; i++)
			for (int j = 0; j < m; j++) cin >> cards[j][i];
		long long ans = 0;
		for (int i = 0; i < m; i++) {
			sort(cards[i], cards[i] + n);
			long long add = -n + 1;
			for (int j = 0; j < n; j++) {
				ans += add * cards[i][j];
				add += 2;
			}
		}
		cout << ans << "\n";
	}
}