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

bool cmp(const pair<int, int> &a, const pair<int, int> &b) {
	if (a.f == b.f) return a.s > b.s;
	return a.f < b.f;
}

multiset<pair<int, int>, decltype(cmp) *> dragons(cmp);

int main() {
	setIO("");
	int s, n;
	cin >> s >> n;
	for (int i = 0; i < n; i++) {
		int x, y;
		cin >> x >> y;
		dragons.insert({x, y});
	}
	for (auto i: dragons) {
//		cout << "curr strength: " << s << " dragon stregnth: " << i.f;
		if (s > i.f) s += i.s;
		else {
			cout << "NO";
			return 0;
		}
	}
	cout << "YES";
}