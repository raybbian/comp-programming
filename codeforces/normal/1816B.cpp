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
		int n;
		cin >> n;
		vector<vector<int>> out(2, vector<int>(n, 0));
		for (int i = 0; i < n; i++) {
			out[(i + 1) % 2][i] = i + 1;
		}
		out[0][0] = 2 * n;
		out[1][n - 1] = 2 * n - 1;
		for (int i = 1; i < n - 1; i++) {
			out[i % 2][i] = n + i;
		}
		
		for (auto &i: out) {
			for (auto &j: i) cout << j << " ";
			cout << "\n";
		}
	}
}