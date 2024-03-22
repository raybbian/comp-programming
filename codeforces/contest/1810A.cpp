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
		int n;
		cin >> n;
		int arr[n];
		string ans = "NO";
		for (int i = 0; i < n; i++) {
			cin >> arr[i];
			if (arr[i] <= i + 1) {
				ans = "YES";
			}
		}
		cout << ans << "\n";
	}
}