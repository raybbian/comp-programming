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

multiset<pair<double, int>> cities;

int main() {
	setIO("");
	int n, s;
	cin >> n >> s;
	for (int i = 0; i < n; i++) {
		int x, y, k;
		cin >> x >> y >> k;
		cities.insert({sqrt(pow(x, 2) + pow(y, 2)), k});
	}
	
	cout.precision(15);
	for (auto &i: cities) {
		s += i.s;
		if (s >= 1000000) {
			cout << i.f;
			return 0;
		}
	}
	cout << "-1";
}