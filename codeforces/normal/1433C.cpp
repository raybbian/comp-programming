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

const int mxN = 3e5 + 5;

int main() {
	setIO("");
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int n;
		vector<pair<int, pair<int, bool>>> piranha;
		cin >> n;
		for (int j = 0; j < n; j++) {
			int a;
			cin >> a;
			piranha.push_back({a, {j, false}});
		}
		for (int j = 0; j < n; j++) {
			if (j != 0) {
				if (piranha[j - 1].f < piranha[j].f) {
					piranha[j].s.s = true;
				} else if (piranha[j - 1].f > piranha[j].f) {
					piranha[j - 1].s.s = true;
				}
			} else if (j != n - 1) {
				if (piranha[j].f < piranha[j + 1].f) {
					piranha[j].s.s = true;
				} else if (piranha[j].f > piranha[j + 1].f) {
					piranha[j + 1].s.s = true;
				}
			}
		}
		sort(piranha.begin(), piranha.end());
		int ans = -2;
		while (!piranha.empty() && ans == -2) {
			if (piranha.back().s.s) {
				ans = piranha.back().s.f;
			} else {
				piranha.pop_back();
			}
		}
		cout << ans + 1 << "\n";
	}
}