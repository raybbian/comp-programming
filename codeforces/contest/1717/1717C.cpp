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
	for (int i = 0; i < t; i++) {
		int n;
		cin >> n;
		bool works = true;
		vector<int> a(n), b(n);
		for (int j = 0; j < n; j++) {
			cin >> a[j];
		}
		for (int j = 0; j < n; j++) {
			cin >> b[j];
			if (b[j] < a[j]) works = false;
		}
		
		for (int j = n - 1; j >= 0; j--) {
			if (b[(j - 1 + n) % n] - b[j] > 1 && a[(j - 1 + n) % n] != b[(j - 1 + n) % n]) {
				works = false;
			}
		}
		cout << (works ? "YES" : "NO") << "\n";
	}
	
}