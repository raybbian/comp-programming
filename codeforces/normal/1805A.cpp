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
		int arr[n], b[n];
		for (int i = 0; i < n; i++) {
			cin >> arr[i];
		}
		int ans = -1;
		for (int i = 0; i <= 256; i++) {
			int running = 0;
			for (int j = 0; j < n; j++) {
				int temp = arr[j] ^ i;
				running ^= temp;
			}
			if (running == 0) {
				ans = i;
				break;
			}
		}
		cout << ans << "\n";
	}
}