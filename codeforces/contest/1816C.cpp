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
		long long arr[n], rev[n];
		for (int i = 0; i < n; i++) {
			cin >> arr[i];
			rev[i] = arr[i];
		}
		
		for (int i = n - 2; i >= 1; i--) {
			long long diff = abs(arr[i] - arr[i + 1]);
			if (arr[i] < arr[i + 1]) {
				arr[i] += diff;
				arr[i - 1] += diff;
			} else if (arr[i] > arr[i + 1]) {
				arr[i] -= diff;
				arr[i - 1] -= diff;
			}
		}
		
		for (int i = 1; i < n - 1; i++) {
			long long diff = abs(arr[i] - arr[i - 1]);
			if (arr[i - 1] < arr[i]) {
				arr[i] -= diff;
				arr[i + 1] -= diff;
			} else if (arr[i - 1] > arr[i]) {
				arr[i] += diff;
				arr[i + 1] += diff;
			}
		}
		
		cout << ((arr[0] > arr[1] || arr[n - 1] < arr[n - 2]) ? "NO\n" : "YES\n");
	}
}