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

set<int> nums;

int main() {
	setIO("");
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		nums.insert(a);
	}
	
	for (auto &i: nums) {
		if (i % *nums.begin() != 0) {
			cout << -1 << "\n";
			return 0;
		}
	}
	
	cout << *nums.begin() << "\n";
}