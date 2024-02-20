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
		vector<int> b(n), a(n + 1);
		for (int i = 1; i < n; i++) {
			cin >> b[i];
		}
		//if the max increases, then the second value is the max
		//if the max decreases, then the first value is the max
		for (int i = 1; i < n; i++) { //for b
			//if b1 < b2, then a3 must be b2, a1 or a2 is b1
			if (i + 1 < n && b[i] < b[i + 1]) {
				a[i + 2] = b[i + 1];
				a[i + 1] = b[i];
			}
				//if b1 > b2, then a1 must be b1
			else if (i + 1 < n && b[i] > b[i + 1]) {
				a[i] = b[i];
				a[i + 1] = b[i + 1];
			} else if ((i + 1 < n && b[i] == b[i + 1]) || n - 1 == 1) {
				a[i + 1] = b[i];
			}
		}
		
		for (int i = 1; i <= n; i++) cout << a[i] << " ";
		cout << "\n";
	}
}