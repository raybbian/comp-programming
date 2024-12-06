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
	//gcd = 1
	int t;
	cin >> t;
	while (t--) {
		int a, b;
		cin >> a >> b;
		//15, 1
		//14, 1
		//1, 0
		
		//1, 15
		//0, 1
		//1, 14
		cout << 2 << "\n";
		cout << a - 1 << " " << 1 << "\n";
		cout << a << " " << b << "\n";
	}
}