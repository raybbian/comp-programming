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
		int n, count = 0;
		cin >> n;
		// 1, 3, 7, 15
		// 1, 1, 1, 1
		// 1, 3, 5, 9
		// 101 -> 51 ->
		vector<int> ans;
		bool works = true;
		while (n > 1 && count <= 40) {
			if (n % 2 == 0) {
				cout << -1 << "\n";
				works = false;
				break;
			}
			if ((n + 1) / 2 % 2 == 1) {
				ans.push_back(1);
				n = (n + 1) / 2;
			} else if ((n - 1) / 2 % 2 == 1) {
				ans.push_back(2);
				n = (n - 1) / 2;
			}
			count++;
		}
		if (works && n == 1) {
			cout << count << "\n";
			for (auto it = ans.rbegin(); it != ans.rend(); it++) cout << *it << " ";
			cout << "\n";
		}
	}
}