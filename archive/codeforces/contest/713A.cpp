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

map<string, int> nums;

int main() {
	setIO("");
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		char c;
		string a, b;
		cin >> c >> a;
		
		//convert a into a pattern
		bool nonZero = false;
		for (auto &j: a) {
			if (!nonZero && char(int(j) % 2 + 48) != '0') nonZero = true;
			if (nonZero) b += char(int(j) % 2 + 48);
		}
		
		if (c == '-') nums[b]--;
		else if (c == '+') nums[b]++;
		else {
			cout << nums[b] << "\n";
		}
	}
}