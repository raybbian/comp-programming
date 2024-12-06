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
	int n;
	cin >> n;
	bool phaseOne = true;
	int minPhaseOne = INT_MAX;
	vector<int> nums;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		nums.push_back(a);
	}
	
	int prev = INT_MIN;
	int ans = -1;
	for (int i = 0; i < n; i++) {
		
		if (nums[i] < prev) {
			//if this number is less than the previous number, then it must be lower than minPhaseOne and must continue strictly increasing
//			cout << "phase two at " << i << " index and minphaseone " << minPhaseOne << "\n";
			if (phaseOne) ans = n - i;
			phaseOne = false;
		}
		if (phaseOne) { //if it is still phase one
			minPhaseOne = min(minPhaseOne, nums[i]);
		}
		if (!phaseOne) {
			if (!(nums[i] <= minPhaseOne && (nums[i] >= prev || ans == n - i))) {
				ans = -1;
				break;
			}
		}
		
		prev = nums[i];
	}
	if (phaseOne) cout << "0\n";
	else cout << ans << "\n";
}

/* strictly increasing -> already sorted
 * strictly decreasing -> n - 1
 * increasing to top, decreasing from bottom ??*/