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

map<int, int> nums;

int main() {
	setIO("");
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		nums[a]++;
	}
	
	for (auto &i: nums) {
//		cout << i.f << " " << i.s << "\n";
	}
	
	for (auto i = nums.begin(); i != nums.end(); i++) {
		if (i != prev(nums.end())) {
			if (nums[i->f] == 0) {
				cout << "NO" << "\n";
				return 0;
			}
			if (next(i)->f - i->f != 1) {
				cout << "NO" << "\n";
				if (n == 472) cout << "gap";
				return 0;
			}
			if (nums[i->f] < nums[next(i)->f]) {
				nums[next(i)->f] -= i->s;
				nums[i->f] = 0;
			} else {
				nums[i->f] -= next(i)->s;
				nums[next(i)->f] = 0;
			}
		}
		
	}
	
	for (auto &i: nums) {
		if (i.s != 0) {
			cout << "NO" << "\n";
			if (n == 472) cout << "chain";
			return 0;
		}
	}
	cout << "YES" << "\n";
}

//two numbers are odd
//rest numbers are even, must be +-1 with all other numbers