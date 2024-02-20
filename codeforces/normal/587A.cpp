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
	int n;
	cin >> n;
	unordered_map<int, int> freq;
	for (int i = 0; i < n; i++) {
		int w;
		cin >> w;
		freq[w]++;
	}
	int ans = 0;
	for (int i = 0; i < 1e6 + 21; i++) {
		if (freq.count(i) == 0) continue;
		freq[i + 1] += freq[i] / 2;
		ans += freq[i] % 2;
	}
	cout << ans << "\n";
}

/* need to double the exponent
 * for example 2^3 needs another 8 more worth of weights
 * two 2^0 combines to 2^1
 * two 2^1 combines to 2^2
 *
 * what about combinations?
 * 2^3 = 2^2 + 2^2 = 2^1 + 2^1 + 2^2
 *
 * just combine the weights to "ascend" to a higher level weight
 *
 * 4 of the same weight can be carried in one go
 * 7 of the same weight needs to be carried in 3 goes
 * each combination can reduce the answer by one move
 * */