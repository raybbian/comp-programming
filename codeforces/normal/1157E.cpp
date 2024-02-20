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

vector<int> a;
multiset<int> b;

int main() {
	setIO("");
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		int x;
		cin >> x;
		a.push_back(x);
	}
	for (int i = 0; i < n; i++) {
		int y;
		cin >> y;
		b.insert(y);
	}
	
	for (auto &i: a) {
		auto it = b.lower_bound(n - i);
		if (it == b.end()) {
			it = b.lower_bound(0);
		}
		
		cout << (i + *it) % n << " ";
		b.erase(it);
	}
}