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
		int n, s1, s2;
		cin >> n >> s1 >> s2;
		pair<long long, long long> r[n];
		for (int i = 0; i < n; i++) {
			cin >> r[i].f;
			r[i].s = i;
		}
		sort(r, r + n);
		vector<long long> q1, q2;
		for (int i = n - 1; i >= 0; i--) {
//			cout << "append to 1 takes: " << r[i].f * s1 * (q1.size() + 1) << "\n";
//			cout << "append to 2 takes: " << r[i].f * s2 * (q2.size() + 1) << "\n";
			if (r[i].f * s1 * (q1.size() + 1) < r[i].f * s2 * (q2.size() + 1)) {
				q1.push_back(r[i].s);
			} else if (r[i].f * s1 * (q1.size() + 1) > r[i].f * s2 * (q2.size() + 1)) {
				q2.push_back(r[i].s);
			} else if (s1 < s2) {
				q1.push_back(r[i].s);
			} else {
				q2.push_back(r[i].s);
			}
		}
		cout << q1.size() << " ";
		for (auto &i: q1) cout << i + 1 << " ";
		cout << "\n" << q2.size() << " ";
		for (auto &i: q2) cout << i + 1 << " ";
		cout << "\n";
	}
}