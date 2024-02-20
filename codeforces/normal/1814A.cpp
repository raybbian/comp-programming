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
		long long n, k;
		cin >> n >> k;
		// 2 * x + k * y == n
		//if k is even and n is odd = no
		//else yes
		if (k % 2 == 0 && n % 2 == 1) cout << "NO\n";
		else cout << "YES\n";
	}
}