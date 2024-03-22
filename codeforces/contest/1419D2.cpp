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

multiset<int> small, large;

int main() {
	setIO("");
	int n;
	cin >> n;
	int ice[n];
	for (int i = 0; i < n; i++) {
		int a;
		cin >> a;
		ice[i] = a;
	}
	sort(ice, ice + n);
	for (int i = 0; i < n; i++) {
		if (i < n / 2) {
			small.insert(ice[i]);
		} else {
			large.insert(ice[i]);
		}
	}
	
	int list[n];
	auto smIt = small.begin(), lgIt = large.begin();
	for (int i = 0; i < n; i++) {
		if (i % 2 == 0) {
			list[i] = *lgIt;
			lgIt++;
		} else {
			list[i] = *smIt;
			smIt++;
		}
	}
	
	int ans = 0;
	for (int i = 1; i < n - 1; i++) if (list[i] < list[i - 1] && list[i] < list[i + 1]) ans++;
	cout << ans << "\n";
	for (int i = 0; i < n; i++) cout << list[i] << " ";
	cout << "\n";
}