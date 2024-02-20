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

const int mxR = 1e6 + 1;
int n;
pair<int, int> t[2 * mxR];

pair<int, int> combine(pair<int, int> a, pair<int, int> b) {
	if (a.f > b.f) return {a.f, a.s};
	else return {b.f, b.s};
}

void build() {  // build the tree
	for (int i = n - 1; i > 0; --i) t[i] = combine(t[i << 1], t[i << 1 | 1]);
}

int query(int l, int r) {  // sum on interval [l, r)
	pair<int, int> res = {0, -1};
	for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
		if (l & 1) res = combine(res, t[l++]);
		if (r & 1) res = combine(res, t[--r]);
	}
	return res.s;
}

int luckiness(int num) {
	int mn = INT_MAX, mx = 0;
	while (num > 0) {
		mn = min(mn, num % 10);
		mx = max(mx, num % 10);
		num /= 10;
	}
	return mx - mn;
}

int main() {
	setIO("");
	int cases;
	cin >> cases;
	n = mxR;
	for (int i = 1; i < mxR; i++) {
		t[mxR + i] = {luckiness(i), i};
	}
	build();
	
	for (int i = 0; i < cases; i++) {
		int a, b;
		cin >> a >> b;
		cout << query(a, b + 1) << "\n";
	}
}