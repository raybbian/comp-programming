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

bool cmp(const pair<char, int> &a, const pair<char, int> &b) {
	if (a.f == b.f) return a.s > b.s;
	return a.f < b.f;
}

int main() {
	setIO("");
	int t;
	cin >> t;
	while (t--) {
		int n;
		string s;
		cin >> n >> s;
		set<pair<char, int>, decltype(cmp) *> nums(cmp);
		for (int i = 0; i < n; i++) {
			nums.insert({s[i], i});
		}
		
		string ans;
		ans += nums.begin()->f;
		for (int i = 0; i < n; i++) {
			if (i == nums.begin()->s) {
				continue;
			}
			ans += s[i];
		}
		cout << ans << "\n";
	}
}