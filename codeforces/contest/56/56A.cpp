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

unordered_set<string> s = {"ABSINTH", "BEER", "BRANDY", "CHAMPAGNE", "GIN", "RUM", "SAKE", "TEQUILA", "VODKA",
						   "WHISKEY", "WINE"};

int main() {
	setIO("");
	int n, ans = 0;
	cin >> n;
	for (int i = 0; i < n; i++) {
		string a;
		cin >> a;
		bool isString = false;
		for (auto &j: a) {
			if (!isdigit(j)) {
				isString = true;
			}
		}
		if (isString) {
			if (s.count(a)) { //if is alcoholic
				ans++;
			}
		} else if (stoi(a) < 18) {
			ans++;
		}
		
	}
	cout << ans << "\n";
}