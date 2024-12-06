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

//custom comparator example
bool cmp(const pair<int, int> &a, const pair<int, int> &b) {
	int lena = a.s - a.f + 1;
	int lenb = b.s - b.f + 1;
	if (lena == lenb) return a.f < b.f;
	return lena > lenb;
}

set<pair<int, int>, decltype(cmp) *> segs(cmp);

int main() {
	setIO("");
	int t;
	cin >> t;
	for (int i = 0; i < t; i++) {
		int n;
		cin >> n;
		segs.insert({0, n - 1});
		vector<int> arr;
		for (int j = 1; j <= n; j++) arr.push_back(0);
		int count = 1;
		while (!segs.empty()) {
			int l = segs.begin()->f;
			int r = segs.begin()->s;
			segs.erase(segs.begin());
			if (l > r) continue;
//			cout << l << " " << r << "\n";
			if ((r - l + 1) % 2 == 0) {
				arr[(l + r - 1) / 2] = count++;
				segs.insert({l, ((l + r - 1) / 2) - 1});
				segs.insert({((l + r - 1) / 2) + 1, r});
			} else {
				arr[(l + r) / 2] = count++;
				segs.insert({l, ((l + r) / 2) - 1});
				segs.insert({((l + r) / 2) + 1, r});
			}
		}
		for (auto &j: arr) {
			cout << j << " ";
		}
		cout << "\n";
	}
	
}