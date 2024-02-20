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
	for (int i = 0; i < t; i++) {
		int n;
		cin >> n;
		
		multiset<int> nums;
		int minDays = ceil(log2(n + 1) - 1);
		for (int j = 0; j < minDays; j++) {
			nums.insert(int(pow(2, j)));
		}
		int remainder = n - (pow(2, minDays) - 1);
		nums.insert(remainder);
		
		cout << minDays << "\n";
		for (auto j = next(nums.begin()); j != nums.end(); j++) {
			cout << *j - *prev(j) << " ";
		}
	}
}