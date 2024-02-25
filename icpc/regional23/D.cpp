#include <bits/stdc++.h>
using namespace std;

int main() {
	int n;
	cin >> n;
	vector<int> d(n);
	for(int i = 0; i < n; i++) {
		cin >> d[i];
	}
	vector<int> ans;
	for(int k = 1; k < n; k++) {
		if (n % k != 0) continue;
		int groups = n / k;
		vector<int> max_in_group(groups);
		vector<int> min_in_group(groups, (int)1e9);
		for(int i = 0; i < groups; i++) {
			for(int j = k * i; j <= k * (i + 1) - 1; j++) {
				max_in_group[i] = max(max_in_group[i], d[j]);
				min_in_group[i] = min(min_in_group[i], d[j]);
			}
		}

		bool found = true;
		for(int i = 1; i < groups; i++) {
			if (max_in_group[i - 1] > min_in_group[i]) {
				found = false;
				break;
			}
		}
		if (found) {
			ans.push_back(groups);
		}
	}

	if ((int) ans.size() == 0) {
		cout << -1 << '\n';
	} else {
		reverse(ans.begin(), ans.end());
		for(auto &i: ans) {
			cout << i << '\n';
		}
	}
}
